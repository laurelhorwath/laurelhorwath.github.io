// File: ContactTest.java
// Author: Laurel Horwath
// Course: CS 320: Software Testing, Automation, and Quality Assurance
// Date: 2026-03-22
// Description: Unit tests for Contact.java to ensure validation rules and getters/setters work as intended.

package Test;

import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import Contact.Contact;

class ContactTest {

	@Test
	void testValidContactCreation() {
		Contact Contact = new Contact("1234", "John", "Smith", "2104356672", "1234 Not Real Blvd.");
		assertEquals("1234", contact.getId());
		assertEquals("John", Contact.getFirstName());
		assertEquals("Smith", Contact.getLastName());
		assertEquals("2104356672", Contact.getNumber());
		assertEquals("1234 Not Real Blvd.", Contact.getAddress());
	}
	
	@Test 
	void testInvalidIdTooLong() {
		Assertions.assertThrows(IllegalArgumentException.class, ()-> {
			new Contact("12345678900", "John", "Smith", "2104356672", "1234 Not Real Blvd.");
		}); }
	
	@Test 
	void testNullFirstName() {
		Assertions.assertThrows(IllegalArgumentException.class, ()-> {
			new Contact("1234", null, "Smith", "2104356672", "1234 Not Real Blvd.");
		}); }
	
	@Test 
	void testInvalidNumberLength() {
		Assertions.assertThrows(IllegalArgumentException.class, ()-> {
			new Contact("1234", "John", "Smith", "12345", "1234 Not Real Blvd.");
		}); }
	
	@Test
	void testSettersValidation() {
		Contact contact = new Contact("1234", "John", "Smith", "2104356672", "1234 Not Real Blvd.");
		assertThrows(IllegalArgumentException.class, () -> contact.setFirstName(null));
		assertThrows(IllegalArgumentException.class, () -> contact.setLastName("VeryLongLastName"));
		assertThrows(IllegalArgumentException.class, () -> contact.setNumberAddress("123"));
		assertThrows(IllegalArgumentException.class, () -> contact.setFirstName(null));
	}

}
