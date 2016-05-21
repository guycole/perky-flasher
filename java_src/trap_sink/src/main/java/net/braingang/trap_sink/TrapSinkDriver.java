package net.braingang.trap_sink;

import org.snmp4j.smi.UdpAddress;

/**
 *
 */
public class TrapSinkDriver {

    /**
     *
     * @param args
     */
    public static void main(String[] args) throws Exception {
        System.out.println("start");

        TrapSink trapSink = new TrapSink();
        trapSink.listen(new UdpAddress("localhost/162"));

        System.out.println("end");
    }
}
