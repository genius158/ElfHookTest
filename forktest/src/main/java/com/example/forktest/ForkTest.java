package com.example.forktest;

public class ForkTest {
   public static void load(){
       System.loadLibrary("forktest");
   }

    public static native void fork();

}
