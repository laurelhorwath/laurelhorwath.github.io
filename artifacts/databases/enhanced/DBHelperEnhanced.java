//****************************************************************************
// DBHelper.java
// ============
// 
// Author: Laurel Horwath
// Course: CS 360 / CS 499 Enhancement
// Date: 04/05/2026
//
//****************************************************************************

package com.zybooks.inventoryorganizer;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

import java.util.ArrayList;

/*
 * Database helper class responsible for all database operations.
 * Enhanced with search, filtering, and validation.
 */
public class DBHelper extends SQLiteOpenHelper {

    private static final String DB_NAME = "InventoryDB";
    private static final int DB_VERSION = 1;

    // Table names
    private static final String TABLE_USERS = "users";
    private static final String TABLE_ITEMS = "items";

    // User columns
    private static final String COL_USER_ID = "id";
    private static final String COL_USERNAME = "username";
    private static final String COL_PASSWORD = "password";

    // Item columns
    private static final String COL_ITEM_ID = "id";
    private static final String COL_ITEM_NAME = "name";
    private static final String COL_ITEM_QUANTITY = "quantity";

    public DBHelper(Context context) {
        super(context, DB_NAME, null, DB_VERSION);
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        // Create Users table
        db.execSQL("CREATE TABLE " + TABLE_USERS + " (" +
                COL_USER_ID + " INTEGER PRIMARY KEY AUTOINCREMENT, " +
                COL_USERNAME + " TEXT UNIQUE, " +
                COL_PASSWORD + " TEXT)");

        // Create Items table
        db.execSQL("CREATE TABLE " + TABLE_ITEMS + " (" +
                COL_ITEM_ID + " INTEGER PRIMARY KEY AUTOINCREMENT, " +
                COL_ITEM_NAME + " TEXT, " +
                COL_ITEM_QUANTITY + " INTEGER)");
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        db.execSQL("DROP TABLE IF EXISTS " + TABLE_USERS);
        db.execSQL("DROP TABLE IF EXISTS " + TABLE_ITEMS);
        onCreate(db);
    }

    // ---------------- USER METHODS ----------------
    public boolean addUser(String username, String password) {
    	
    	if (username == null || username.isEmpty() || password == null || password.isEmpty()) {
    		return false;
    	}
    	
        SQLiteDatabase db = this.getWritableDatabase();
        ContentValues values = new ContentValues();
        values.put(COL_USERNAME, username);
        values.put(COL_PASSWORD, password);

        long result = db.insert(TABLE_USERS, null, values);
        return result != -1;
    }

    public boolean checkUser(String username, String password) {
        SQLiteDatabase db = this.getReadableDatabase();
        
        Cursor cursor = db.rawQuery(
                "SELECT * FROM " + TABLE_USERS + " WHERE " + COL_USERNAME + "=? AND " + COL_PASSWORD + "=?",
                new String[]{username, password}
        );
        boolean exists = cursor.getCount() > 0;
        cursor.close();
        return exists;
    }

    // ---------------- ITEM METHODS ----------------
    
    /*
     * Insert item with validation.
     */
    public boolean insertItem(String name, int quantity) {
    	
    	if (!Item.isValidName(name) || !Item.isValidQuantity(quantity)) {
    		return false;
    	}
    	
        SQLiteDatabase db = this.getWritableDatabase();
        ContentValues values = new ContentValues();
        values.put(COL_ITEM_NAME, name);
        values.put(COL_ITEM_QUANTITY, quantity);
        
        long result = db.insert(TABLE_ITEMS, null, values);
        return result != 1;
    }
    
    /*
     * Update item with validation
     */
    public boolean updateItem(Item oldItem, String newName, int newQuantity) {
    	
    	if (!Item.isValidName(newName) || !Item.isValidQuantity(newQuantity)) {
    		return false;
    	}
    	
        SQLiteDatabase db = this.getWritableDatabase();
        ContentValues values = new ContentValues();
        values.put(COL_ITEM_NAME, newName);
        values.put(COL_ITEM_QUANTITY, newQuantity);
        
        int rows = db.update(TABLE_ITEMS, values, COL_ITEM_ID + "=?", new String[]{String.valueOf(oldItem.getId())});
        
        return rows > 0;
    }

    public void deleteItem(Item item) {
        SQLiteDatabase db = this.getWritableDatabase();
        db.delete(TABLE_ITEMS, COL_ITEM_ID + "=?", new String[]{String.valueOf(item.getId())});
    }

    /*
     * Get all items
     */
    public ArrayList<Item> getItems() {
    	
        ArrayList<Item> items = new ArrayList<>();
        SQLiteDatabase db = this.getReadableDatabase();
        
        Cursor cursor = db.rawQuery("SELECT * FROM " + TABLE_ITEMS, null);

        if (cursor.moveToFirst()) {
            do {
                int id = cursor.getInt(cursor.getColumnIndexOrThrow(COL_ITEM_ID));
                String name = cursor.getString(cursor.getColumnIndexOrThrow(COL_ITEM_NAME));
                int quantity = cursor.getInt(cursor.getColumnIndexOrThrow(COL_ITEM_QUANTITY));
                
                items.add(new Item(id, name, quantity));
                
            } while (cursor.moveToNext());
        }
        cursor.close();
        return items;
    }
    
 // ---------------- ENAHNCEMENTS ----------------
    
    /*
     * Search items by name
     */
    public ArrayList<Item> searchItems(String searchText) {

        ArrayList<Item> items = new ArrayList<>();
        SQLiteDatabase db = this.getReadableDatabase();

        Cursor cursor = db.rawQuery( "SELECT * FROM " + TABLE_ITEMS + " WHERE " + COL_ITEM_NAME + " LIKE ?", new String[]{"%" + searchText + "%"});

        if (cursor.moveToFirst()) {

            do {

                int id = cursor.getInt(cursor.getColumnIndexOrThrow(COL_ITEM_ID));
                String name = cursor.getString(cursor.getColumnIndexOrThrow(COL_ITEM_NAME));
                int quantity = cursor.getInt(cursor.getColumnIndexOrThrow(COL_ITEM_QUANTITY));

                items.add(new Item(id, name, quantity));

            } while (cursor.moveToNext());
        }

        cursor.close();
        return items;
    }

    /*
     * Filter items by minimum quantity
     */
    public ArrayList<Item> filterItemsByQuantity(int minQuantity) {

        ArrayList<Item> items = new ArrayList<>();
        SQLiteDatabase db = this.getReadableDatabase();

        Cursor cursor = db.rawQuery( "SELECT * FROM " + TABLE_ITEMS + " WHERE " + COL_ITEM_QUANTITY + " >= ?", new String[]{String.valueOf(minQuantity)});

        if (cursor.moveToFirst()) {
        	
            do {

                int id = cursor.getInt(cursor.getColumnIndexOrThrow(COL_ITEM_ID));
                String name = cursor.getString(cursor.getColumnIndexOrThrow(COL_ITEM_NAME));
                int quantity = cursor.getInt(cursor.getColumnIndexOrThrow(COL_ITEM_QUANTITY));

                items.add(new Item(id, name, quantity));

            } while (cursor.moveToNext());
        }

        cursor.close();
        return items;
    }
}