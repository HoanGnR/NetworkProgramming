/*
Name: Trinh Ngoc Huy
Mssv: 16022440

Content:

    -Server listen request from Client and reply them

*/


import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.io.File;
import java.io.BufferedWriter;
import java.io.BufferedReader;
import java.io.OutputStreamWriter;
import java.io.InputStreamReader;
import java.io.FileInputStream;
 
class server{   
 
    public static void main(String[] args) throws IOException {
        int port = 1998; 
        ServerSocket serverSocket = null;
        Socket socket = null;
        OutputStream os = null;
        InputStream is = null;
        OutputStreamWriter osw = null;
        InputStreamReader isr = null;
        BufferedWriter bw = null;
        BufferedReader br = null;
        FileInputStream fis = null;
        try {
            System.out.println("Listening in port " + port + ", wait request from Client ...");
            serverSocket = new ServerSocket(port);
            System.out.println("Server started: " + serverSocket);
            System.out.println("Waiting for a client ...");
            socket = serverSocket.accept();
            System.out.println("Client accepted: " + socket);
            System.out.println();

             // use to read byte
            os = socket.getOutputStream();
            is = socket.getInputStream();

            osw = new OutputStreamWriter(os);
            isr = new InputStreamReader(is);
            
            //use to read string
            bw = new BufferedWriter(osw);
            br = new BufferedReader(isr);
            while (true) {
                //Get request command from Client
                String selected;
                selected = br.readLine();

                // Mode show list file
                if(selected.equals("@ls")){
                    System.out.println();
                    System.out.println("User choose mode show list file in SharedFolder");
                    File file = new File("SharedFolder");
                    File[] listFile = file.listFiles();
                    int countFile = listFile.length;
                    os.write(countFile);
                    int i = 0;
                    while(true){
                        if(i == countFile) break;
                        String information = listFile[i].getName() + " ("+ listFile[i].length() +" bytes)";
                        bw.write(information);
                        bw.newLine();
                        bw.flush();
                        i++;
                        
                    }

                }
                //Mode download file
                else if(selected.equals("@download")){
                    System.out.println();
                    System.out.println("User choose mode download file in SharedFolder");
                    //Get file name from client
                    String fileName = br.readLine();

                    //Check file exists
                    System.out.println();
                    System.out.println("User want download file: "+fileName);
                    File pathFile = new File("SharedFolder/"+fileName);
                    String status;
                    if(pathFile.exists() == false){
                        System.out.println();
                        System.out.println("File doesn't exists !");
                        status = "false";
                        bw.write(status);
                        bw.newLine();
                        bw.flush();
                        continue;
                    }
                    else {
                        System.out.println();
                        System.out.println("File exists !");
                        status = "true";
                        bw.write(status);
                        bw.newLine();
                        bw.flush();
                    }
                    //Send file size
                    long fileSize = pathFile.length();
                    bw.write(Long.toString(fileSize));
                    bw.newLine();
                    bw.flush();


                    // Send file
                    int nbytes;
                    fis = new FileInputStream(pathFile);
                    byte[] bytes = new byte[1024];
                    while((nbytes = fis.read(bytes)) != -1){
                        os.write(bytes,0,nbytes);
                    }
                    System.out.println();
                    System.out.println("Send Successfully !");
                    fis.close();

                }
                //Mode log out
                else if(selected.equals("@logout")){
                    System.out.println();
                    System.out.println("User choose mode log out");
                    System.out.println();

                    break;
                }
                // If command incorrect
                else {
                    System.out.println();
                    System.out.println("InCorrect.");
                }
            }
            bw.close();
            br.close();
            os.close();
            is.close();
        } catch (IOException ie) {
            ie.printStackTrace();
        } finally {
            if (serverSocket != null) {
                serverSocket.close();
            }
        }
    }
 
}