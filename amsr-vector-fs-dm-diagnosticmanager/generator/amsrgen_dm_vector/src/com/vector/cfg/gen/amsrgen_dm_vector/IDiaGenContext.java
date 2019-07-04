package com.vector.cfg.gen.amsrgen_dm_vector;

import com.vector.cfg.gen.amsrgen_base.model.util.MdfServices;
import com.vector.cfg.gen.amsrgen_dm_vector.model.IGenModel;

/**
 *
 * <p>
 * (c) Copyright Vector Informatik GmbH. All Rights Reserved.
 * </p>
 *
 * @since 1.0
 */
public interface IDiaGenContext {

    MdfServices getMdf();

    void registerGenModel(IGenModel model);

}
