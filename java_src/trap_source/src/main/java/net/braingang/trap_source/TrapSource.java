package net.braingang.trap_source;

import org.snmp4j.CommunityTarget;
import org.snmp4j.PDU;
import org.snmp4j.PDUv1;
import org.snmp4j.Snmp;
import org.snmp4j.TransportMapping;
import org.snmp4j.mp.SnmpConstants;
import org.snmp4j.smi.IpAddress;
import org.snmp4j.smi.OID;
import org.snmp4j.smi.OctetString;
import org.snmp4j.smi.UdpAddress;
import org.snmp4j.transport.DefaultUdpTransportMapping;

/**
 *
 */
public class TrapSource {
    public static final int PORT = 162;
    public static final String COMMUNITY = "public";

    public static final String DOTTED = ".1.3.6.1.2.1.1.8";
    public static final String IP_ADDRESS = "127.0.0.1";

    /**
     * generate V1 trap
     */
    public void generateTrap() {
        CommunityTarget target = new CommunityTarget();
        target.setCommunity(new OctetString(COMMUNITY));
        target.setVersion(SnmpConstants.version1);
        target.setAddress(new UdpAddress(IP_ADDRESS + "/" + PORT));
        target.setTimeout(5000);
        target.setRetries(2);

        PDUv1 pdu = new PDUv1();
        pdu.setType(PDU.V1TRAP);
        pdu.setEnterprise(new OID(DOTTED));
        pdu.setGenericTrap(PDUv1.ENTERPRISE_SPECIFIC);
        pdu.setSpecificTrap(1);
        pdu.setAgentAddress(new IpAddress(IP_ADDRESS));

        try {
            TransportMapping transport = new DefaultUdpTransportMapping();
            transport.listen();

            Snmp snmp = new Snmp(transport);
            snmp.send(pdu, target);
            snmp.close();
        } catch (Exception exception) {
            exception.printStackTrace();
        }
    }
}