package com.vector.cfg.gen.amsrgen_dm_vector.model;

import com.vector.cfg.gen.amsrgen_dm_vector.IDiaGenContext;
import com.vector.cfg.util.log.ILogger;
import com.vector.cfg.util.log.Logger;

/**
 *
 * <p>
 * (c) Copyright Vector Informatik GmbH. All Rights Reserved.
 * </p>
 *
 * @since 1.0
 */
public abstract class AbstractGenModelImporter<T> implements IGenModelImporter<T> {
    @SuppressWarnings("unused")
    private static final ILogger logger = Logger.INSTANCE.createLogger(AbstractGenModelImporter.class);

    private final IDiaGenContext context;

    /**
     * Constructor for AbstractModelImporter.
     *
     * @param context
     */
    protected AbstractGenModelImporter(final IDiaGenContext context) {
        this.context = context;
    }

    /**
     * Getter method for context.
     * 
     * @return Returns the context.
     */
    public IDiaGenContext getContext() {
        return context;
    }

}
