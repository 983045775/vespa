// Copyright 2016 Yahoo Inc. Licensed under the terms of the Apache 2.0 license. See LICENSE in the project root.
#include <vespa/fastos/fastos.h>
#include <vespa/vsm/vsm/docsumfieldspec.h>

namespace vsm {

DocsumFieldSpec::FieldIdentifier::FieldIdentifier() :
    _id(StringFieldIdTMap::npos),
    _path()
{
}

DocsumFieldSpec::FieldIdentifier::FieldIdentifier(FieldIdT id, FieldPath path) :
    _id(id),
    _path(path)
{
}


DocsumFieldSpec::DocsumFieldSpec() :
    _resultType(search::docsummary::RES_INT),
    _command(VsmsummaryConfig::Fieldmap::NONE),
    _outputField(),
    _inputFields()
{
}

DocsumFieldSpec::DocsumFieldSpec(search::docsummary::ResType resultType,
                                 VsmsummaryConfig::Fieldmap::Command command) :
    _resultType(resultType),
    _command(command),
    _outputField(),
    _inputFields()
{
}

}

