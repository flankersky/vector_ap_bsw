package com.vector.cfg.gen.amsrgen_dm_vector.model.server;

import java.util.Optional;

import com.vector.cfg.model.mdf.ar4x.adaptiveplatform.softwarecluster.MISoftwareCluster;
import com.vector.cfg.model.mdf.ar4x.adaptiveplatform.softwarecluster.MISoftwareClusterDiagnosticAddress;
import com.vector.cfg.model.mdf.ar4x.adaptiveplatform.softwarecluster.MISoftwareClusterDiagnosticAddressSemanticsEnum;
import com.vector.cfg.model.mdf.ar4x.adaptiveplatform.softwarecluster.MISoftwareClusterDoipDiagnosticAddress;
import com.vector.cfg.model.mdf.ar4x.diagnosticextract.diagnosticcontribution.MIDiagnosticContributionSet;
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
public class SoftwareCluster {
    @SuppressWarnings("unused")
    private static final ILogger logger = Logger.INSTANCE.createLogger(SoftwareCluster.class);

    private final MISoftwareCluster softwareCluster;

    /**
     * Constructor for SoftwareCluster.
     *
     * @param softwareCluster
     */
    public SoftwareCluster(final MISoftwareCluster softwareCluster) {
        this.softwareCluster = softwareCluster;
    }

    public MIDiagnosticContributionSet getDiagnosticExtract() {
        return softwareCluster.getDiagnosticExtract().getRefTarget();
    }

    public String getName() {
        return softwareCluster.getName();
    }

    public int getPhysicalAddress() {
        final Optional<MISoftwareClusterDiagnosticAddress> physAddr = softwareCluster.getDiagnosticAddress().stream()
                .filter(address -> address.getAddressSemantics() == MISoftwareClusterDiagnosticAddressSemanticsEnum.PHYSICAL_ADDRESS).findFirst();
        if (physAddr.isPresent()) {
            final MISoftwareClusterDoipDiagnosticAddress doipAddr = (MISoftwareClusterDoipDiagnosticAddress) physAddr.get();
            return doipAddr.getDiagnosticAddress().intValue();
        } else {
            return -1;
        }
    }
}
