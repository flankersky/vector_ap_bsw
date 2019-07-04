package com.vector.cfg.gen.amsrgen_dm_vector;

import com.vector.cfg.gen.amsrgen_base.AbstractAmsrgenGeneratorPackage;
import com.vector.cfg.gen.amsrgen_dm_vector.calculator.DiaCalculator;
import com.vector.cfg.gen.core.moduleinterface.IGenerationProcessor;
import com.vector.cfg.gen.core.moduleinterface.calculation.GenCalculationAspect;
import com.vector.cfg.gen.core.moduleinterface.calculation.IGenCalculationAspect;
import com.vector.cfg.model.access.DefRef;

/**
 *
 * <p>
 * (c) Copyright Vector Informatik GmbH. All Rights Reserved.
 * </p>
 *
 * @since 1.0
 */
public class GeneratorPackage extends AbstractAmsrgenGeneratorPackage {

    private static final String GENERATOR_VERSION = "1.00.00";

    private static final String MSN = "amsrgen_dm_vector";

    private static final DefRef MODULE_DEFREF = DefRef.create(DefRef.MICROSAR_PACKAGE, MSN);

    /**
     * {@inheritDoc}
     */
    @Override
    public String getMSN() {
        return MSN;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getGeneratorName() {
        return MSN + " Generator";
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getDescriptiveName() {
        return MSN;
    }

    /**
     * Step 1/3 on initialization of generator.
     *
     * {@inheritDoc}
     */
    @Override
    protected void registerGeneratorPackageAspects() {
        // register the calculation class(es) that are able to manipulate the SystemExtract/Mdf model
        addPackageAspect(IGenCalculationAspect.class, GenCalculationAspect.create(new DiaCalculator(this)));
    }

    /**
     * Step 2/3 on initialization of generator.
     *
     * {@inheritDoc}
     */
    @Override
    protected void registerValidators() {
        // TODO Auto-generated method stub

    }

    /**
     * Step 3/3 on initialization of generator.
     *
     * {@inheritDoc}
     */
    @Override
    protected IGenerationProcessor registerGenerationProcessor() {
        return new DiaGenerationProcessor(this);
    }

    /*
     * **************************************************** DO NOT CHANGE THE TEXT BELOW
     ****************************************************/
    private static final String CFG_CORE_FEATURE_VERSION = CFG_CORE_FEATURE_VERSION_STRING;

    /**
     * {@inheritDoc}
     */
    @Override
    public String getCfgCoreFeatureVersionString() {
        return CFG_CORE_FEATURE_VERSION;
    }

    /**
     * Constructor for GeneratorPackage.
     *
     */
    public GeneratorPackage() {
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public DefRef getModuleDefinitionRef() {
        return MODULE_DEFREF;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getGeneratorVersionString() {
        return GENERATOR_VERSION;
    }
    /*****************************************************
     * DO NOT CHANGE THE TEXT ABOVE
     ****************************************************/

}
