package com.zybooks.inventoryorganizer;

public class Item {
    private int id;
    private String name;
    private int quantity;

    public Item(int id, String name, int quantity) {
        this.id = id;
        this.name = name;
        this.quantity = quantity;
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