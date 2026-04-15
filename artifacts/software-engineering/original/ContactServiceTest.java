package Test;

import org.junit.jupiter.api.BeforeEach;
import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.Test;

import Contact.Contact;
import Contact.ContactService;

public class ContactServiceTest {
	private ContactService contactService;
	
	@BeforeEach
	void setUp() {
		contactService = new ContactService();
	}
	
	@Test
	void testAddValidContact() {
		Contact contact = new Contact("1234567890", "John", "Smith", "1234567890", "123 Not Real Blvd.");
		assertTrue(contactService.addContact(contact));
		assertNotNull(contactService.getContact("1234567890"));
	}

	@Test
	void testAddDuplicateContact() {
		Contact contact1 = new Contact("1234567890", "John", "Smith", "1234567890", "123 Not Real Blvd.");
		Contact contact2 = new Contact("1234567890", "Jane", "Doe", "0987654321", "234 Imaginary Ln.");
		
		assertTrue(contactService.addContact(contact1));
		assertFalse(contactService.addContact(contact2));
	
	}
	@Test 
	 void testDeleteContact() {
		Contact contact = new Contact("1234567890", "John", "Smith", "1234567890", "123 Not Real Blvd.");
		contactService.addContact(contact);
		
		assertTrue(contactService.deleteContact("1234567890"));
		assertNull(contactService.getContact("1234567890"));
		
	}
	
	@Test
	void testDeleteNonExistentContact() {
		assertFalse(contactService.deleteContact("1111111111"));
	}
	
	@Test 
	void testUpdateFirstName() {
		Contact contact = new Contact("1234567890" , "John", "Smith", "1234567890", "123 Not Real Blvd.");
		contactService.addContact(contact);
		
		assertTrue(contactService.updateFirstName("1234567890",  "Jane"));
		assertEquals("Jane", contactService.getContact("1234567890").getFirstName());
		
	}
	
	@Test 
	void testUpdateNonExistentContact() {
		assertFalse(contactService.updateFirstName("9999999999",  "Test"));
		
	}
	
	@Test 
	void testUpdateNumber() {
		Contact contact = new Contact("1234567890", "John", "Smith", "1234567890", "123 Not Real Blvd.");
		contactService.addContact(contact);
		
		assertTrue(contactService.updateNumber("1234567890",  "0987654321"));
		assertEquals("0987654321", contactService.getContact("1234567890").getNumber());
	}
	
	@Test
	void testUpdateLastName() {
		Contact contact = new Contact("1234567890", "John", "Smith", "1234567890", "123 Not Real Blvd.");
		contactService.addContact(contact);
		
		assertTrue(contactService.updateLastName("1234567890", "Doe"));
		assertEquals("Doe", contactService.getContact("1234567890").getLastName());
	}
	
	@Test
	void testUpdateAddress() {
		Contact contact = new Contact("1234567890", "John", "Smith", "1234567890", "123 Not Real Blvd.");
		contactService.addContact(contact);
		
		assertTrue(contactService.updateAddress("1234567890", "456 Fictional St."));
		assertEquals("456 Fictional St.", contactService.getContact("1234567890").getAddress());
	}

}
