package net.braingang.trap_sink;

import junit.framework.Test;
import junit.framework.TestCase;
import junit.framework.TestSuite;
import org.snmp4j.smi.UdpAddress;

/**
 * Unit test for simple TrapSinkDriver.
 */
public class TrapSinkDriverTest extends TestCase {
    /**
     * Create the test case
     *
     * @param testName name of the test case
     */
    public TrapSinkDriverTest(String testName )
    {
        super( testName );
    }

    /**
     * @return the suite of tests being tested
     */
    public static Test suite()
    {
        return new TestSuite( TrapSinkDriverTest.class );
    }

    /**
     *
     */
    public void testApp() {
        // assertTrue( true );

        try {
            TrapSink trapSink = new TrapSink();
        //    trapSink.listen(new UdpAddress("localhost/162"));
        } catch(Exception exception) {
            exception.printStackTrace();
          //  assertTrue(false);
        }
    }
}
