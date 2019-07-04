package com.vector.cfg.gen.amsrgen_dm_vector;

import java.util.ArrayList;
import java.util.List;

import com.vector.cfg.gen.amsrgen_base.AbstractAmsrgenGenerationProcessor;
import com.vector.cfg.gen.amsrgen_dm_vector.generator.GenOutputWriter;
import com.vector.cfg.gen.amsrgen_dm_vector.model.BaseConfigModel;
import com.vector.cfg.gen.amsrgen_dm_vector.model.IGenModel;
import com.vector.cfg.gen.core.moduleinterface.IGeneratorPackage;
import com.vector.cfg.gen.core.moduleinterface.IGeneratorResultSink;
import com.vector.cfg.gen.core.moduleinterface.fileoutput.IGenerationOutputWriter;
import com.vector.cfg.util.log.ILogger;
import com.vector.cfg.util.log.Logger;

/**
 * Do the actual generation work.
 * <ul>
 * <li>calculate() - Write calculated values to the SystemExtract
 * <li>buildDataStructures() - Create and validate the data for generation
 * <li>generate() - Do the generation
 * <li>cleanDataStructures() - Cleanup the created structures (mostly executed before buildDataStructures() is called)
 * </ul>
 * <p>
 * (c) Copyright Vector Informatik GmbH. All Rights Reserved.
 * </p>
 *
 * @since 1.0
 */
public class DiaGenerationProcessor extends AbstractAmsrgenGenerationProcessor {
    @SuppressWarnings("unused")
    private static final ILogger logger = Logger.INSTANCE.createLogger(DiaGenerationProcessor.class);

    /**
     * Constructor.
     */
    public DiaGenerationProcessor(final IGeneratorPackage generatorPackage) {
        super(generatorPackage);
    }

    private final List<IGenModel> generatorModels = new ArrayList<>();

    /**
     * {@inheritDoc}
     */
    @Override
    public void buildDataStructures(final IGeneratorResultSink resultSink) {
        // update and validate models already created in Calculation phase
        for (final IGenModel model : generatorModels) {
            model.updateAndValidate(resultSink);
        }

        // Create additional models
        final IDiaGenContext context = DiaGenContext.newContext(getGeneratorPackage());
        // Build BaseConfig
        final BaseConfigModel baseConfig = new BaseConfigModel(context);
        baseConfig.buildAndValidate(resultSink);
        registerGenModel(baseConfig);
    }

    /**
     *
     * {@inheritDoc}
     */
    @Override
    public void generate(final IGeneratorResultSink resultSink, final IGenerationOutputWriter writer) {

        // output writer collects output files
        final GenOutputWriter genOutputWriter = new GenOutputWriter(getGeneratorPackage());

        // generate output for registered models
        for (final IGenModel model : generatorModels) {
            model.generate(resultSink, genOutputWriter);
        }

        genOutputWriter.createOutput(writer);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void cleanDataStructures() {
        generatorModels.clear();
    }

    public void registerGenModel(final IGenModel model) {
        generatorModels.add(model);
    }

}
