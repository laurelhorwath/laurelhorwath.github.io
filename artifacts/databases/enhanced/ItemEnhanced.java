//****************************************************************************
// Item.java
// ============
// 
// Author: Laurel Horwath
// Course: CS 360 / CS 499 Enhancement
// Date: 04/05/2026
// 
//****************************************************************************

package com.zybooks.inventoryorganizer;

/*
 * Represents a single inventory item. 
 * Includes basic validation to ensure data integrity.
 */

public class Item {
    
	private int id;
    private String name;
    private int quantity;
    
    /*
     * Constructor with validation
     */

    public Item(int id, String name, int quantity) {
    	if (!isValidName(name)) {
    		throw new IllegalArgumentException("Invalid item name.");
    	}
    	if (!isValidQuantity(quantity)) {
    		throw new IllegalArgumentException("Quantity must be >= 0");
    	}
    
        this.id = id;
        this.name = name;
        this.quantity = quantity;
    }
    
    // Validation methods
    
    public static boolean isValidName(String name) {
    	return name != null && !name.trimm().isEmpty();
    }
    
    public static boolean isValidQuantity(int quantity) {
    	return quantity >= 0;
    	
    }

    // Getters
    public int getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public int getQuantity() {
        return quantity;
    }

    @Override
    public String toString() {
        return name + " (Qty: " + quantity + ")";
    }
}