// File: ContactService.java
// Author: Laurel Horwath
// Course: CS 320: Software Testing, Automation, and Quality Assurance
// Date: 2026-03-22
// Description: Manages a collection of Contact objects using a HashMap for efficient retrieval,
//              addition, deletion, and update operations.

package Contact;

import java.util.HashMap;
import java.util.Map;

public class ContactService {
	
	// Using HashMap for 0(1) access by ID
	private final Map<String, Contact> contacts = new HashMap<>();
	
	
	// ADD CONTACT
	public boolean addContact(Contact contact) {
		if (contacts.containsKey(contact.getId())) {
			return false; // ID already exists
		}
		contacts.put(contact.getId(), contact);
		return true;
	}
	
	// DELETE CONTACT
	public boolean deleteContact(String id) {
		Contact contact = getContact(id);
		if (contacts.containsKey(id)) {
			contacts.remove(id);
			return true;
		}
		return false; // Contact not found
	}
	
	// UPDATE CONTACT FIELDS
	public boolean updateFirstName(String id, String firstName) {
		Contact contact = contacts.get(id);
		if (contact != null) {
			contact.setFirstName(firstName);
			return true;
			} 
		return false;
		}
    public boolean updateLastName(String id, String lastName) {
	    Contact contact = contacts.get(id);
	    if (contact != null) {
		    contact.setLastName(lastName);
		    return true;
	    }
	    return false;
    }
    public boolean updateNumber(String id, String number) {
	    Contact contact = contacts.get(id);
	    if (contact != null) {
		    contact.setNumber(number);
		    return true;
	    }
	    return false;
    }
    public boolean updateAddress(String id, String address) {
	    Contact contact = contacts.get(id);
	    if (contact != null) {
		    contact.setAddress(address);
		    return true;
	    }
	    return false;
    }
    
    // GET CONTACT BY ID
    public Contact getContact(String id) {
	    return contacts.get(id);
    }

    // PRINT ALL CONTACTS
    public void printAllContacts() {
	    for (Contact contact : contacts.values()) {
		    System.out.println(contact);
		
		    }
	    }
    }
