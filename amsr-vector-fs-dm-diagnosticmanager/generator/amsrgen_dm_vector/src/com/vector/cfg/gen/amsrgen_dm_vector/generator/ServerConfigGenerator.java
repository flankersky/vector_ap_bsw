package com.vector.cfg.gen.amsrgen_dm_vector.generator;

import com.vector.cfg.gen.amsrgen_base.model.json.JsonObject;
import com.vector.cfg.gen.amsrgen_base.model.json.JsonObjectBuilder;
import com.vector.cfg.gen.amsrgen_dm_vector.DiaConstants;
import com.vector.cfg.gen.amsrgen_dm_vector.model.server.ServerModel;
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
public class ServerConfigGenerator {
    @SuppressWarnings("unused")
    private static final ILogger logger = Logger.INSTANCE.createLogger(ServerConfigGenerator.class);

    private final ServerModel serverModel;

    /**
     * Constructor for ServerGenerator.
     *
     * @param serverModel
     */
    public ServerConfigGenerator(final ServerModel serverModel) {
        this.serverModel = serverModel;
    }

    public void generate(final IGeneratorResultSink resultSink, final GenOutputWriter writer) {
        final JsonObjectBuilder jb = new JsonObjectBuilder();
        jb.add("TargetAddress", serverModel.getSoftwareCluster().getPhysicalAddress());
        final JsonObject serverConfig = jb.build();

        writer.setBaseFolder(DiaConstants.DM_GEN_ROOT_FOLDER);
        writer.addServerConfigJson(serverModel.getName(), serverConfig);
    }
}
