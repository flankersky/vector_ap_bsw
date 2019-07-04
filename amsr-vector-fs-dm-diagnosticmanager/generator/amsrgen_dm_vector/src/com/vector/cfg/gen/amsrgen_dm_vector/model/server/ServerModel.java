package com.vector.cfg.gen.amsrgen_dm_vector.model.server;

import com.vector.cfg.gen.amsrgen_dm_vector.IDiaGenContext;
import com.vector.cfg.gen.amsrgen_dm_vector.generator.GenOutputWriter;
import com.vector.cfg.gen.amsrgen_dm_vector.generator.ServerConfigGenerator;
import com.vector.cfg.gen.amsrgen_dm_vector.model.AbstractGenModel;
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
public class ServerModel extends AbstractGenModel implements IServerModel {
    @SuppressWarnings("unused")
    private static final ILogger logger = Logger.INSTANCE.createLogger(ServerModel.class);

    private SoftwareCluster softwareCluster;

    /**
     * Constructor for ServerModel.
     *
     * @param iDiaGenContext
     */
    ServerModel(final IDiaGenContext context) {
        super(context);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void generate(final IGeneratorResultSink resultSink, final GenOutputWriter writer) {
        final ServerConfigGenerator serverConfigGenerator = new ServerConfigGenerator(this);
        serverConfigGenerator.generate(resultSink, writer);
    }

    /**
     * Getter method for softwareCluster.
     *
     * @return Returns the softwareCluster.
     */
    @Override
    public SoftwareCluster getSoftwareCluster() {
        return softwareCluster;
    }

    /**
     * Setter method for softwareCluster.
     *
     * @param softwareCluster The softwareCluster to set.
     */
    public void setSoftwareCluster(final SoftwareCluster softwareCluster) {
        this.softwareCluster = softwareCluster;
    }

    public String getName() {
        return softwareCluster.getName();
    }
}
