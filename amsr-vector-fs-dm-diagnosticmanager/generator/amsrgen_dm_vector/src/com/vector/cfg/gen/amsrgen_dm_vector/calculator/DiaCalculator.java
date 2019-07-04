package com.vector.cfg.gen.amsrgen_dm_vector.calculator;

import java.util.ArrayList;
import java.util.Collection;
import java.util.List;

import com.vector.cfg.gen.amsrgen_dm_vector.model.server.IServerModel;
import com.vector.cfg.gen.amsrgen_dm_vector.model.server.ServerModelImporter;
import com.vector.cfg.gen.core.moduleinterface.IGeneratorPackageReferable;
import com.vector.cfg.gen.core.moduleinterface.IGeneratorResultSink;
import com.vector.cfg.gen.core.moduleinterface.calculation.ECalculationSubPhase;
import com.vector.cfg.model.mdf.ar4x.adaptiveplatform.softwarecluster.MISoftwareCluster;
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
public class DiaCalculator extends AbstractDiaCalculator {
    @SuppressWarnings("unused")
    private static final ILogger logger = Logger.INSTANCE.createLogger(DiaCalculator.class);

    List<IServerModel> serverModels = new ArrayList<>();

    /**
     * Constructor for DiagnosticsModelModifier.
     *
     * @param generatorPackage
     */
    public DiaCalculator(final IGeneratorPackageReferable generatorPackage) {
        super(generatorPackage, ECalculationSubPhase.MODULE_INTERNAL_CALCULATION);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void calculate(final IGeneratorResultSink resultSink) {

        createServerModels(resultSink);

        // check basic DM model (-> AbstractGeneratorModel)

        // register all models for further actions
        for (final IServerModel serverModel : serverModels) {
            getGenContext().registerGenModel(serverModel);
        }

    }

    /**
     * @param resultSink
     */
    private void createServerModels(final IGeneratorResultSink resultSink) {
        // search for SoftwareCluster
        final Collection<MISoftwareCluster> softwareClusters = getGenContext().getMdf().getAllInstancesOfType(MISoftwareCluster.class);

        // for each SoftwareCluster, create ServerModel (-> AbstractGeneratorModel)
        for (final MISoftwareCluster softwareCluster : softwareClusters) {
            final ServerModelImporter importer = new ServerModelImporter(softwareCluster, getGenContext());
            final IServerModel serverModel = importer.doImport();
            serverModels.add(serverModel);
        }
    }

}
