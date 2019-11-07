 /*
Name: Trinh Ngoc Huy
Mssv: 16022440

Content:

    -Client send request to Server with three option:
      -- @ls to show list file in specific folder (SharedFoler)
      -- @download to download file in specific folder (SharedFoler)
      -- @logout to log out and close connection.

*/

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.util.Scanner;
import java.io.BufferedWriter;
import java.io.BufferedReader;
import java.io.OutputStreamWriter;
import java.io.InputStreamReader;
import java.io.FileOutputStream;
 
class client{
    public static void main(String[] args) throws IOException, InterruptedException {

        String ipServer;
        String fileName;
        int port = 1998;
        Scanner scan = new Scanner(System.in);
        Socket socket = null;
        OutputStream os = null;
        InputStream is = null;
        OutputStreamWriter osw = null;
        InputStreamReader isr = null;
        BufferedWriter bw = null;
        BufferedReader br = null;
        FileOutputStream fos = null;

        try {
            System.out.print("IP Server: ");
            ipServer = scan.nextLine();
            System.out.println();            
            socket = new Socket(ipServer, port); // Connect to server
            System.out.println("Connected: " + socket);
            System.out.println();
            //use to read, write number and byte
            os = socket.getOutputStream();
            is = socket.getInputStream();

            osw = new OutputStreamWriter(os);
            isr = new InputStreamReader(is);
            
            //use to read, write string
            bw = new BufferedWriter(osw);
            br = new BufferedReader(isr);


            while(true){
              System.out.println("____________________.Choose action.______________________");
              System.out.println("|                                                       |");
              System.out.println("|   @ls. -to show list file in folder SharedFolder      |");
              System.out.println("|   @download. -to download file in folder SharedFolder |");
              System.out.println("|   @logout. -to log out                                |");
              System.out.println("_________________________________________________________");
              System.out.println();
              System.out.print("Your command: ");

              // Get command insert from keyboard and send to Server
              String selected;
              selected = scan.nextLine();
              bw.write(selected);
              bw.newLine();
              bw.flush();

              // Mode show list file
              if(selected.equals("@ls")){
                int countFile;
                countFile = is.read();
                System.out.println();
                System.out.println("______________________Show List File____________________");
                System.out.println("=> Total file: "+countFile);
                System.out.println("=> List File in folder SharedFolder on Server:");
                int i = 0;
                while(true){
                  if(i == countFile)  break;
                  fileName = br.readLine();
                  System.out.println(" ~> "+fileName);
                  i++;
                }
                System.out.println("__________________________________________________________");
                System.out.println();

              }

              //Mode download file
              else if(selected.equals("@download")){
                System.out.println();
                System.out.println("______________________Download File______________________");
                long currentFileSize = 0;
                System.out.println();
                System.out.print("Choose file name: ");
                fileName = scan.nextLine();
                bw.write(fileName);
                bw.newLine();
                bw.flush();
                
                //Read check file message from server
                String status = br.readLine();
                if(status.equals("false")){
                  System.out.println();
                  System.out.println("ERR: File doesn't exists. Please try again.");
                  System.out.println();
                  continue;
                }
                else {
                  System.out.println("Preparing to download file...");
                }


                // Receive file size
                String fileSizeInString = br.readLine();
                long fileSize = Integer.parseInt(fileSizeInString);

                // Receive file
                int nbytes;
                fos = new FileOutputStream(fileName);
                byte[] bytes = new byte[1024];
                while(currentFileSize < fileSize){
                  nbytes =is.read(bytes);
                  fos.write(bytes,0,nbytes);
                  currentFileSize = currentFileSize + nbytes;
                }
                System.out.println("Download Successfully !");
                System.out.println();
                fos.close();
                System.out.println("__________________________________________________________");

              }
              //Mode log out
              else if(selected.equals("@logout")){
                System.out.println();
                System.out.println("_________________________Log Out___________________________");
                System.out.println("        Kill the process and close connection.");
                System.out.println("___________________________________________________________");
                System.out.println();
                break;
              }
              // If command incorrect
              else {
                System.out.println();
                System.out.println("ERR: Incorrect.");
                System.out.println();

              }

            }
            bw.close();
            br.close();
            os.close();
            is.close();
        } catch (IOException ie) {
            System.out.println("Can't connect to server");
        } finally {
            if (socket != null) {
                socket.close();
            }
        }
    }
}