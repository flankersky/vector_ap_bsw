
package com.vector.cfg.gen.amsrgen_dm_vector.generator;

import com.vector.cfg.gen.amsrgen_base.AbstractGenOutputWriter;
import com.vector.cfg.gen.amsrgen_base.model.json.JsonStructure;
import com.vector.cfg.gen.amsrgen_base.outputfiles.AdaptiveOutputFile;
import com.vector.cfg.gen.amsrgen_base.outputfiles.jetargs.IJetArgument;
import com.vector.cfg.gen.amsrgen_dm_vector.outputfiles.jetargs.DmBaseConfig_JsonJetArg;
import com.vector.cfg.gen.amsrgen_dm_vector.outputfiles.jetargs.DmServerConfig_JsonJetArg;
import com.vector.cfg.gen.amsrgen_dm_vector.outputfiles.jetclasses.DmBaseConfig_JsonJet;
import com.vector.cfg.gen.amsrgen_dm_vector.outputfiles.jetclasses.DmServerConfig_JsonJet;
import com.vector.cfg.gen.core.moduleinterface.IGeneratorPackage;
import com.vector.cfg.util.log.ILogger;
import com.vector.cfg.util.log.Logger;

/**
 *
 * <p>
 * (c) Copyright Vector Informatik GmbH. All Rights Reserved.
 * </p>
 *
 */
public class GenOutputWriter extends AbstractGenOutputWriter {
    @SuppressWarnings("unused")
    private static final ILogger logger = Logger.INSTANCE.createLogger(GenOutputWriter.class);

    /**
     * Constructor.
     */
    public GenOutputWriter(final IGeneratorPackage generatorPackage) {
        super(generatorPackage);
    }

    public void addBaseConfigJson(final JsonStructure jsonModel) {
        final IJetArgument dataRep = new DmBaseConfig_JsonJetArg(getGeneratorPackage(), jsonModel, getBaseFolder());
        addOutputFile(AdaptiveOutputFile.of(new DmBaseConfig_JsonJet(), dataRep));
    }

    public void addServerConfigJson(final String serverName, final JsonStructure jsonModel) {
        final IJetArgument dataRep = new DmServerConfig_JsonJetArg(getGeneratorPackage(), jsonModel, getBaseFolder(), serverName);
        addOutputFile(AdaptiveOutputFile.of(new DmServerConfig_JsonJet(), dataRep));
    }

}
