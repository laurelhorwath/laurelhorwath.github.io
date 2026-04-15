package Contact;

import java.util.ArrayList;
import java.util.List;

public class ContactService {
	private final List<Contact> contacts = new ArrayList<>();
	
	public boolean addContact(Contact contact) {
		if (getContact(contact.getId()) != null) {
			return false;
		}
		contacts.add(contact);
		return true;
		}
	public boolean deleteContact(String id) {
		Contact contact = getContact(id);
		if (contact != null) {
			contacts.remove(contact);
			return true;
		}
		return false;
	}

public boolean updateFirstName(String id, String firstName) {
	Contact contact = getContact(id);
	if (contact != null) {
		contact.setFirstName(firstName);
		return true;
	} 
	return false;
}
public boolean updateLastName(String id, String lastName) {
	Contact contact = getContact(id);
	if (contact != null) {
		contact.setLastName(lastName);
		return true;
	}
	return false;
}
public boolean updateNumber(String id, String number) {
	Contact contact = getContact(id);
	if (contact != null) {
		contact.setNumber(number);
		return true;
	}
	return false;
}
public boolean updateAddress(String id, String address) {
	Contact contact = getContact(id);
	if (contact != null) {
		contact.setAddress(address);
		return true;
	}
	return false;
}
public Contact getContact(String id) {
	for (Contact contact : contacts) {
		if (contact.getId().equals(id)) {
			return contact;
		}
	}
	return null;
}
public void printAllContacts() {
	for (Contact contact : contacts) {
		System.out.println(contact);
		
		}
	}
}
