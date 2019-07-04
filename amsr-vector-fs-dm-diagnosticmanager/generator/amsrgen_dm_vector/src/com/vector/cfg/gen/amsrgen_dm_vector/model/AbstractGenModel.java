package com.vector.cfg.gen.amsrgen_dm_vector.model;

import com.vector.cfg.gen.amsrgen_dm_vector.IDiaGenContext;
import com.vector.cfg.gen.core.moduleinterface.IGeneratorResultSink;
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
public abstract class AbstractGenModel implements IGenModel {
    @SuppressWarnings("unused")
    private static final ILogger logger = Logger.INSTANCE.createLogger(AbstractGenModel.class);

    private final IDiaGenContext genContext;

    /**
     * Constructor for AbstractGenModel.
     *
     * @param genOutputWriter
     * @param iDiaGenContext
     */
    public AbstractGenModel(final IDiaGenContext iDiaGenContext) {
        this.genContext = iDiaGenContext;
    }

    protected IDiaGenContext getContext() {
        return genContext;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateAndValidate(final IGeneratorResultSink resultSink) {
        // by default do nothing
    }

}
