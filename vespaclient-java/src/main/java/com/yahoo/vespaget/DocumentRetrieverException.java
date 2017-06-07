// Copyright 2017 Yahoo Inc. Licensed under the terms of the Apache 2.0 license. See LICENSE in the project root.
package com.yahoo.vespaget;

/**
 * Exception thrown by {@link DocumentRetriever}.
 *
 * @author bjorncs
 */
public class DocumentRetrieverException extends Exception {

    public DocumentRetrieverException(String message) {
        super(message);
    }
}