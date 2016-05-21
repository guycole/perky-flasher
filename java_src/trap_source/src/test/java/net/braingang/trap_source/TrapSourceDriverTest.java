package net.braingang.trap_source;

import junit.framework.Test;
import junit.framework.TestCase;
import junit.framework.TestSuite;

/**
 * Unit test for simple TrapSourceDriver.
 */
public class TrapSourceDriverTest extends TestCase {
    /**
     * Create the test case
     *
     * @param testName name of the test case
     */
    public TrapSourceDriverTest(String testName )
    {
        super( testName );
    }

    /**
     * @return the suite of tests being tested
     */
    public static Test suite()
    {
        return new TestSuite( TrapSourceDriverTest.class );
    }

    /**
     *
     */
    public void testApp() {
//        assertTrue( true );

        TrapSource trapSource = new TrapSource();
        trapSource.generateTrap();
    }
}
