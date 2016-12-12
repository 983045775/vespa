// Copyright 2016 Yahoo Inc. Licensed under the terms of the Apache 2.0 license. See LICENSE in the project root.

#include "clusterstate.h"
#include <vespa/vdslib/state/clusterstate.h>
#include <vespa/vdslib/distribution/distribution.h>
#include <vespa/vespalib/objects/nbostream.h>
#include <vespa/vespalib/stllike/asciistream.h>

namespace storage {
namespace spi {

ClusterState::ClusterState(const lib::ClusterState& state,
                           uint16_t nodeIndex,
                           const lib::Distribution& distribution)
    : _state(new lib::ClusterState(state)),
      _nodeIndex(nodeIndex),
      _distribution(new lib::Distribution(distribution.serialize()))
{

}

void
ClusterState::deserialize(vespalib::nbostream& i)
{
    vespalib::string clusterState;
    vespalib::string distribution;

    i >> clusterState;
    i >> _nodeIndex;
    i >> distribution;

    _state.reset(new lib::ClusterState(clusterState));
    _distribution.reset(new lib::Distribution(distribution));
}

ClusterState::ClusterState(vespalib::nbostream& i)
{
    deserialize(i);
}

ClusterState::ClusterState(const ClusterState& other)
{
    vespalib::nbostream o;
    other.serialize(o);
    deserialize(o);
}

ClusterState::~ClusterState() { }

ClusterState&
ClusterState::operator=(const ClusterState& other)
{
    ClusterState copy(other);
    _state = std::move(copy._state);
    _nodeIndex = copy._nodeIndex;
    _distribution = std::move(copy._distribution);
    return *this;
}

bool
ClusterState::shouldBeReady(const Bucket& b) const
{
    assert(_distribution.get());
    assert(_state.get());

    if (_distribution->getReadyCopies() >= _distribution->getRedundancy()) {
        return true; // all copies should be ready
    }

    std::vector<uint16_t> idealNodes;
    _distribution->getIdealNodes(lib::NodeType::STORAGE, *_state,
                                 b.getBucketId(), idealNodes,
                                 "uim", _distribution->getReadyCopies());
    for (uint32_t i=0, n=idealNodes.size(); i<n; ++i) {
        if (idealNodes[i] == _nodeIndex) return true;
    }
    return false;
}

bool
ClusterState::clusterUp() const
{
    return _state.get() && _state->getClusterState() == lib::State::UP;
}

bool
ClusterState::nodeUp() const
{
    return _state.get() &&
        _state->getNodeState(lib::Node(lib::NodeType::STORAGE, _nodeIndex)).
        getState().oneOf("uir");
}

bool
ClusterState::nodeInitializing() const
{
    return _state.get() &&
        _state->getNodeState(lib::Node(lib::NodeType::STORAGE, _nodeIndex)).
        getState().oneOf("i");
}

void
ClusterState::serialize(vespalib::nbostream& o) const
{
    assert(_distribution.get());
    assert(_state.get());
    vespalib::asciistream tmp;
    _state->serialize(tmp, false);
    o << tmp.str() << _nodeIndex;
    o << _distribution->serialize();
}

} // spi
} // storage
