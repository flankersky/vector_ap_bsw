package com.vector.cfg.gen.amsrgen_dm_vector.calculator;

import com.vector.cfg.gen.amsrgen_dm_vector.DiaGenContext;
import com.vector.cfg.gen.amsrgen_dm_vector.IDiaGenContext;
import com.vector.cfg.gen.core.moduleinterface.IGeneratorPackageReferable;
import com.vector.cfg.gen.core.moduleinterface.calculation.ECalculationSubPhase;
import com.vector.cfg.gen.core.utils.abstractImpl.AbstractCalculator;
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
public abstract class AbstractDiaCalculator extends AbstractCalculator {
    @SuppressWarnings("unused")
    private static final ILogger logger = Logger.INSTANCE.createLogger(AbstractDiaCalculator.class);

    private final IDiaGenContext context;

    /**
     * Constructor for AbstractModelModifier.
     *
     * @param generatorPackageRef
     * @param associatedSubPhase
     */
    protected AbstractDiaCalculator(final IGeneratorPackageReferable generatorPackageRef, final ECalculationSubPhase associatedSubPhase) {
        super(generatorPackageRef, associatedSubPhase);
        context = DiaGenContext.newContext(generatorPackageRef.getGeneratorPackage());
    }

    /**
     * Getter method for context.
     *
     * @return Returns the context.
     */
    public IDiaGenContext getGenContext() {
        return context;
    }
}
