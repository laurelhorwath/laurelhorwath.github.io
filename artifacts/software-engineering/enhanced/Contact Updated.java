// File: Contact.java
// Author: Laurel Horwath
// Course: CS 320: Software Testing, Automation, and Quality Assurance
// Date: 2026-03-22
// Description: Represents a Contact object with fields for ID, first name,
//              last name, phone number, and address. Includes validation rules and reusable 
//              methods to ensure data integrity. Used in a contact management system.

package Contact;

public class Contact {
	
	private String id;
	private String firstName;
	private String lastName;
	private String number;
	private String address;
	
	// Constructor
	public Contact(String id, String firstName, String lastName, String number, String address) {
		validateId(id);
		this.id = id;
		
		setFirstName(firstName); 
		setLastName(lastname);
		setNumber(number);
		SetAddress(address);
	}
	
	// GETTERS
	public String getId() {
		return id;
	}
	public String getFirstName() {
		return firstName;
	}
	public String getLastName() {
		return lastName;
	}
	public String getNumber() {
		return number;
	}
	public String getAddress() {
		return address;
	}
	
	// SETTERS
	
	public void setFirstName(String firstName) {
		validateName(firstName, "First name");
		this.firstName = firstName;
	}
	public void setLastName(String lastName) {
		validateName(lastName, "Last name");
		this.lastName = lastName;
	}
	public void setNumber(String number) {
		validateNumber(number);
		this.number = number;
	}
	public void setAddress(String address) {
		validateAddress(address);
		this.address = address;
	}
	
	// VALIDATION METHODS
	private void validateId(String Id) {
		if (id == null) throw new IllegalArgumentException("ID cannot be null");
		if (id.length() > 10) throw new IllegalArgumentException("ID cannot exceed 10 characters");
	}
	private void validateName(String name, String fieldName) {
		if (name == null) throw new IllegalArgumentException(fieldName + " cannot be null");
		if (name.length() > 10) throw new IllegalArgumentException(fieldName + " cannot exceed 10 characters");
	}
	private void validateNumber(String number) {
		if (number == null) throw new IllegalArgumentException("Number cannot be null");
		if (number.length() != 10) throw new IllegalArgumentException("Number must be exactly 10 characters");
	}
	private void validateAddress(String address) {
		if (address == null) throw new IllegalArgumentException("Address cannot be null");
		if (address.length() > 30) throw new IllegalArgumentException("Address cannot exceed 30 characters");
	}
	
	// TO STRING 
	@Override
	public String toString() {
		return "Contact{" +
	"id=' " + id + '\' ' + 
	", firstName=' " + firstName + '\' ' +
	", lastName=' " + lastName + '\' ' +
	", number=' " + number + '\' ' +
	", address=' " + address + '\' ' +
	'}';
	}
}

