package com.vector.cfg.gen.amsrgen_dm_vector.model;

import com.vector.cfg.gen.amsrgen_base.model.json.JsonObject;
import com.vector.cfg.gen.amsrgen_base.model.json.JsonObjectBuilder;
import com.vector.cfg.gen.amsrgen_base.model.json.JsonStructure;
import com.vector.cfg.gen.amsrgen_dm_vector.DiaConstants;
import com.vector.cfg.gen.amsrgen_dm_vector.IDiaGenContext;
import com.vector.cfg.gen.amsrgen_dm_vector.generator.GenOutputWriter;
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
public class BaseConfigModel extends AbstractGenModel {
    @SuppressWarnings("unused")
    private static final ILogger logger = Logger.INSTANCE.createLogger(BaseConfigModel.class);

    private JsonStructure baseConfig;

    /**
     * Constructor for BaseConfigModel.
     *
     * @param baseFolder
     */
    public BaseConfigModel(final IDiaGenContext context) {
        super(context);
    }

    /**
     * {@inheritDoc}
     */
    public void buildAndValidate(final IGeneratorResultSink resultSink) {
        JsonObjectBuilder jb = new JsonObjectBuilder();
        jb.add("MaxPayloadBytes", 13);
        final JsonObject logOuputInternal = jb.build();
        jb = new JsonObjectBuilder();
        jb.add("LogOutput", logOuputInternal);
        baseConfig = jb.build();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void generate(final IGeneratorResultSink resultSink, final GenOutputWriter writer) {
        writer.setBaseFolder(DiaConstants.DM_GEN_ROOT_FOLDER);
        writer.addBaseConfigJson(baseConfig);
    }

}
