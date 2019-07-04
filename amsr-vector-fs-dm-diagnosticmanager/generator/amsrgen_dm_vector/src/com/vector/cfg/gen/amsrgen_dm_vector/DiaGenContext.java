package com.vector.cfg.gen.amsrgen_dm_vector;

import com.vector.cfg.gen.amsrgen_base.model.util.MdfServices;
import com.vector.cfg.gen.amsrgen_dm_vector.model.IGenModel;
import com.vector.cfg.gen.core.moduleinterface.IGeneratorPackage;
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
public class DiaGenContext implements IDiaGenContext {
    @SuppressWarnings("unused")
    private static final ILogger logger = Logger.INSTANCE.createLogger(DiaGenContext.class);

    private final MdfServices mdf;

    final private IGeneratorPackage generatorPackage;

    private DiaGenerationProcessor generationProcessor;

    public static IDiaGenContext newContext(final IGeneratorPackage generatorPackage) {
        return new DiaGenContext(generatorPackage);
    }

    private DiaGenContext(final IGeneratorPackage generatorPackage) {
        this.generatorPackage = generatorPackage;
        this.mdf = MdfServices.of(generatorPackage.getProjectContext());
    }

    private DiaGenerationProcessor getGenerationProcessor() {
        if (generationProcessor == null) {
            // Initialize generationProcessor.
            generationProcessor = (DiaGenerationProcessor) generatorPackage.getGenerationProcessor();
        }
        return generationProcessor;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public MdfServices getMdf() {
        return mdf;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void registerGenModel(final IGenModel model) {
        getGenerationProcessor().registerGenModel(model);
    }

}
