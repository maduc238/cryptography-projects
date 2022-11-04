package com.maduc.salsa20;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

// @SpringBootApplication
public class Application {

	public static void main(String[] args) {
		// SpringApplication.run(Application.class, args);
		Salsa20 sa = new Salsa20("*Thirty-two byte (256 bits) key*", "12345678");
		sa.encrypt("Attack at dawn");
	}

}