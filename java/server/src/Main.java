
import java.rmi.AlreadyBoundException;
import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.Locale;

public class Main {

    public static void main(String[] args) throws Exception {
        RemoteServer server = new RemoteServer();
        Registry registry = LocateRegistry.createRegistry(Constants.RMI_PORT);
        registry.bind(Constants.RMI_ID, server);
        System.out.println("Server is up!");
    }
}
