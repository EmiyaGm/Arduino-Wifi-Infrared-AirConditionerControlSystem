package com.gm.tcp;

import java.io.*;

import java.net.*;
import java.sql.Connection;
import java.sql.Statement;

import java.applet.Applet;

import com.gm.db.DB;

public class Server{
    public static void main(String args[]){
    try{
        ServerSocket server=null;
        try{
            server=new ServerSocket(8080);
            System.out.println("Socket success");
            //����һ��ServerSocket�ڶ˿�8080�����ͻ�����
        }catch(Exception e) {

             System.out.println("can not listen to:"+e);

             //������ӡ������Ϣ

         }

        Socket socket=null;
     try{

        socket=server.accept();//ʹ��accept()�����ȴ��ͻ������пͻ�
        //�����������һ��Socket���󣬲�����ִ��
        System.out.println("client success");
        }catch(Exception e) {
        	System.out.println("Error."+e);//������ӡ������Ϣ
        	}
   //  String line;
     BufferedReader is=new BufferedReader(new InputStreamReader(socket.getInputStream()));
     //��Socket����õ�����������������Ӧ��BufferedReader����
     PrintWriter os=new PrintWriter(socket.getOutputStream());
     //��Socket����õ��������������PrintWriter����
    // BufferedReader sin=new BufferedReader(new InputStreamReader(System.in));
     //��ϵͳ��׼�����豸����BufferedReader����
    System.out.println("Client:"+is.readLine());
     //�ڱ�׼����ϴ�ӡ�ӿͻ��˶�����ַ���
     //line=sin.readLine();
     //�ӱ�׼�������һ�ַ���
     while(true){
    	 //������ַ���Ϊ "bye"����ֹͣѭ��
    	// os.println(line);
    	 //��ͻ���������ַ���
    	// os.flush();
    	 //ˢ���������ʹClient�����յ����ַ���
    	// System.out.println("Server:"+line);
    	 //��ϵͳ��׼����ϴ�ӡ������ַ���
		 
    	 if(is.readLine()!="\r"){
    		 String temp;
    		 temp=is.readLine();
    		 System.out.println("Client:"+temp); 
    		 String sql="insert into esp8266 values('"+temp+"');";
    		 try{
    			 Statement stmt = null;
    			 Connection conn = null;
    			 conn=DB.getConn();
    			 stmt=DB.createStmt(conn);
    			 stmt.execute(sql);
    			 }
    		 catch(Exception e){
    			 e.printStackTrace();
    			 }
    		 }
    	
 
    	 //Thread.sleep(15000);
    	 //��Client����һ�ַ���������ӡ����׼�����
    	// line=sin.readLine();
    	 //��ϵͳ��׼�������һ�ַ���
    	 }//����ѭ��
     //os.close(); //�ر�Socket�����
   //  is.close(); //�ر�Socket������
   //  socket.close(); //�ر�Socket
   //  server.close(); //�ر�ServerSocket
     }catch(Exception e){
    	 System.out.println("ERROE is:"+e);
    	 }
    //������ӡ������Ϣ
    }
}