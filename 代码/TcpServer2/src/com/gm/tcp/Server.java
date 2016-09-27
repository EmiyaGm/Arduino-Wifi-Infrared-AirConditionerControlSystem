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
            //创建一个ServerSocket在端口8080监听客户请求
        }catch(Exception e) {

             System.out.println("can not listen to:"+e);

             //出错，打印出错信息

         }

        Socket socket=null;
     try{

        socket=server.accept();//使用accept()阻塞等待客户请求，有客户
        //请求到来则产生一个Socket对象，并继续执行
        System.out.println("client success");
        }catch(Exception e) {
        	System.out.println("Error."+e);//出错，打印出错信息
        	}
   //  String line;
     BufferedReader is=new BufferedReader(new InputStreamReader(socket.getInputStream()));
     //由Socket对象得到输入流，并构造相应的BufferedReader对象
     PrintWriter os=new PrintWriter(socket.getOutputStream());
     //由Socket对象得到输出流，并构造PrintWriter对象
    // BufferedReader sin=new BufferedReader(new InputStreamReader(System.in));
     //由系统标准输入设备构造BufferedReader对象
    System.out.println("Client:"+is.readLine());
     //在标准输出上打印从客户端读入的字符串
     //line=sin.readLine();
     //从标准输入读入一字符串
     while(true){
    	 //如果该字符串为 "bye"，则停止循环
    	// os.println(line);
    	 //向客户端输出该字符串
    	// os.flush();
    	 //刷新输出流，使Client马上收到该字符串
    	// System.out.println("Server:"+line);
    	 //在系统标准输出上打印读入的字符串
		 
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
    	 //从Client读入一字符串，并打印到标准输出上
    	// line=sin.readLine();
    	 //从系统标准输入读入一字符串
    	 }//继续循环
     //os.close(); //关闭Socket输出流
   //  is.close(); //关闭Socket输入流
   //  socket.close(); //关闭Socket
   //  server.close(); //关闭ServerSocket
     }catch(Exception e){
    	 System.out.println("ERROE is:"+e);
    	 }
    //出错，打印出错信息
    }
}