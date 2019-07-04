package com.vector.cfg.gen.amsrgen_dm_vector.model;

import com.vector.cfg.gen.amsrgen_dm_vector.DiaGenerationProcessor;
import com.vector.cfg.gen.amsrgen_dm_vector.generator.GenOutputWriter;
import com.vector.cfg.gen.core.moduleinterface.IGeneratorResultSink;

/**
 *
 * <p>
 * (c) Copyright Vector Informatik GmbH. All Rights Reserved.
 * </p>
 *
 * @since 1.0
 */
public interface IGenModel {

    /**
     * Update the model and validate it.
     * <p>
     * This method is used for models created during Calculation Phase and registered to {@link DiaGenerationProcessor}. This method is called during Validation
     * Phase in {@link DiaGenerationProcessor#buildDataStructures(IGeneratorResultSink)}.
     * </p>
     *
     * @param resultSink
     */
    public void updateAndValidate(IGeneratorResultSink resultSink);

    /**
     * Generate artifacts.
     *
     * @param resultSink
     */
    public void generate(IGeneratorResultSink resultSink, final GenOutputWriter genOutputWriter);
}
