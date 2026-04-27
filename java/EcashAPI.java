public class EcashAPI {
    static {
        System.loadLibrary("ecash_jni");
    }

    public native String initSystem();
    public native String withdrawCoin();
    public native String viewWallet();
    public native String spendCoin();
    public native String verifyPayment();
    public native String doubleSpendDemo();
}