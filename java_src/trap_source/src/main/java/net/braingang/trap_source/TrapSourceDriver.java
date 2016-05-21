package net.braingang.trap_source;

/**
 *
 */
public class TrapSourceDriver {

    /**
     *
     * @param args
     */
    public static void main(String[] args) {
        System.out.println("start");

        TrapSource trapSource = new TrapSource();
        trapSource.generateTrap();

        System.out.println("end");
    }
}
