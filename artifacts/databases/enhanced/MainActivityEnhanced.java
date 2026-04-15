//****************************************************************************
// MainActivity.java
// ============
// 
// Author: Laurel Horwath
// Course: CS 360 / CS 499 Enhancement
// Date: 04/05/2026
// 
//****************************************************************************

package com.zybooks.inventoryorganizer;

import androidx.appcompat.app.AppCompatActivity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

/*
 * Handles user login and registration with validation.
 */
public class MainActivity extends AppCompatActivity {

    EditText username, password;
    Button loginBtn, registerBtn;
    DBHelper db;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        username = findViewById(R.id.editTextUsername);
        password = findViewById(R.id.editTextPassword);
        loginBtn = findViewById(R.id.buttonLogin);
        registerBtn = findViewById(R.id.buttonCreateAccount);

        db = new DBHelper(this);

        // Handle Login
        loginBtn.setOnClickListener(v -> {
            String user = username.getText().toString().trim();
            String pass = password.getText().toString().trim();
            
            if (user.isEmpty() || pass.isEmpty()) {
            	Toast.makeText(MainActivity.this, "Fields cannot be empty", Toast.LENGTH_SHORT).show();
            	return;
            }
            
            if (db.checkUser(user, pass)) {
            	Toast.makeText(MainActivity.this, "Login successful!", Toast.LENGTH_SHORT).show();
            	startActivity(new Intent(MainActivity.this, InventoryActivity.class));    
            } else {
                Toast.makeText(MainActivity.this, "Invalid login", Toast.LENGTH_SHORT).show();
            }
        });

        // Handle Register
        registerBtn.setOnClickListener(v -> {
        	String user = username.getText().toString().trim();
            String pass = password.getText().toString().trim();
            
            if (user.isEmpty() || pass.isEmpty()) {
            	Toast.makeText(MainActivity.this, "Field cannot be empty", Toast.LENGTH_SHORT).show();
            	return;
            }
            
            if (db.addUser(user, pass)) {
                Toast.makeText(MainActivity.this, "Account created!", Toast.LENGTH_SHORT).show();
            } else {
                Toast.makeText(MainActivity.this, "User already exists", Toast.LENGTH_SHORT).show();
            }
        });
    }
}