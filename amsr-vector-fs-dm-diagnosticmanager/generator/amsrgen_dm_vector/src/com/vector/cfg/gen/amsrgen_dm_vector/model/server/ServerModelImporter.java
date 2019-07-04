package com.vector.cfg.gen.amsrgen_dm_vector.model.server;

import com.vector.cfg.gen.amsrgen_dm_vector.IDiaGenContext;
import com.vector.cfg.gen.amsrgen_dm_vector.model.AbstractGenModelImporter;
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
public class ServerModelImporter extends AbstractGenModelImporter<IServerModel> {
    @SuppressWarnings("unused")
    private static final ILogger logger = Logger.INSTANCE.createLogger(ServerModelImporter.class);

    private final MISoftwareCluster softwareCluster;

    /**
     * Constructor for ServerImporter.
     *
     * @param softwareCluster
     * @param iDiaGenContext
     */
    public ServerModelImporter(final MISoftwareCluster softwareCluster, final IDiaGenContext iDiaGenContext) {
        super(iDiaGenContext);
        this.softwareCluster = softwareCluster;
    }

    @Override
    public IServerModel doImport() {
        softwareCluster.getDiagnosticExtract().getRefTargets();
        final ServerModel server = new ServerModel(getContext());
        server.setSoftwareCluster(new SoftwareCluster(softwareCluster));
        return server;
    }

}
