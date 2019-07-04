package com.vector.cfg.gen.amsrgen_dm_vector.outputfiles.jetargs;

import com.vector.cfg.gen.amsrgen_base.model.json.JsonStructure;
import com.vector.cfg.gen.amsrgen_base.outputfiles.jetargs.JetArgumentJson;
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
public class DmBaseConfig_JsonJetArg extends JetArgumentJson {
    @SuppressWarnings("unused")
    private static final ILogger logger = Logger.INSTANCE.createLogger(DmBaseConfig_JsonJetArg.class);

    private final String baseFolder;

    /**
     * Constructor for DmBaseConfig_JsonJetArg.
     *
     * @param generatorPackage
     * @param jsonModel
     */
    public DmBaseConfig_JsonJetArg(final IGeneratorPackage generatorPackage, final JsonStructure jsonModel, final String baseFolder) {
        super(generatorPackage, jsonModel);
        this.baseFolder = baseFolder;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getFilename() {
        return "dm_base_config.json";
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getSubFolder() {
        return baseFolder;
    }

}
