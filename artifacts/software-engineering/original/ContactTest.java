package Test;

import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import Contact.Contact;

class ContactTest {

	@Test
	void testContact() {
		Contact Contact = new Contact("1234", "John", "Smith", "2104356672", "1234 Not Real Blvd.");
		assertTrue(Contact.getId().equals("1234"));
		assertTrue(Contact.getFirstName().equals("John"));
		assertTrue(Contact.getLastName().equals("Smith"));
		assertTrue(Contact.getNumber().equals("2104356672"));
		assertTrue(Contact.getAddress().equals("1234 Not Real Blvd."));
	}
	
	@Test 
	void testContactIdTooLong() {
		Assertions.assertThrows(IllegalArgumentException.class, ()-> {
			new Contact("12345678900", "John", "Smith", "2104356672", "1234 Not Real Blvd.");
		}); }
	
	@Test 
	void testContactIdNull() {
		Assertions.assertThrows(IllegalArgumentException.class, ()-> {
			new Contact(null, "John", "Smith", "2104356672", "1234 Not Real Blvd.");
		}); }
	
	@Test 
	void testContactFirstNameTooLong() {
		Assertions.assertThrows(IllegalArgumentException.class, ()-> {
			new Contact("1234", "JohnJohnJoh", "Smith", "2104356672", "1234 Not Real Blvd.");
		}); }
	
	@Test 
	void testContactFirstNameNull() {
		Assertions.assertThrows(IllegalArgumentException.class, ()-> {
			new Contact("1234", null, "Smith", "2104356672", "1234 Not Real Blvd.");
		}); }
	
	@Test 
	void testContactLastNameTooLong() {
		Assertions.assertThrows(IllegalArgumentException.class, ()-> {
			new Contact("1234", "John", "SmithSmithS", "2104356672", "1234 Not Real Blvd.");
		}); }
	
	@Test 
	void testContactLastNameNull() {
		Assertions.assertThrows(IllegalArgumentException.class, ()-> {
			new Contact("1234", "John", null, "2104356672", "1234 Not Real Blvd.");
		}); }
	
	@Test 
	void testContactNumberTooLong() {
		Assertions.assertThrows(IllegalArgumentException.class, ()-> {
			new Contact("1234", "John", "Smith", "21043566722", "1234 Not Real Blvd.");
		}); }
	
	@Test 
	void testContactNumberTooShort() {
		Assertions.assertThrows(IllegalArgumentException.class, ()-> {
			new Contact("1234", "John", "Smith", "210435667", "1234 Not Real Blvd.");
		}); }
	
	@Test 
	void testContactNumberNull() {
		Assertions.assertThrows(IllegalArgumentException.class, ()-> {
			new Contact("1234", "John", "Smith", null, "1234 Not Real Blvd.");
		}); }
	
	@Test 
	void testContactAddressTooLong() {
		Assertions.assertThrows(IllegalArgumentException.class, ()-> {
			new Contact("1234", "John", "Smith", "2104356672", "1234 Not Real Blvd. 782312 SATX");
		}); }
	
	@Test
	void testSetFirstNameNull() {
		Contact contact = new Contact("1234", "John", "Smith", "2104356672", "1234 Not Real Blvd.");
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			contact.setFirstName(null);
		}); }
	
	@Test 
	void testSetFirstNameTooLong() {
		Contact contact = new Contact ("1234", "John", "Smith", "2104356672", "1234 Not Real Blvd.");
		Assertions.assertThrows(IllegalArgumentException.class () -> {
			contact.setFirstName("JohnJohnJoh");
		}); }
	@Test
	void testSetLastNameNull() {
		Contact contact = new Contact("1234", "John", "Smith", "2104356672", "1234 Not Real Blvd.");
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			contact.setLastName(null);
		}); }	
	@Test
	void testSetLastNameTooLong() {
		Contact contact = new Contact("1234", "John", "Smith", "2104356672", "1234 Not Real Blvd.");
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			contact.setLastName("SmithSmithS");
	    }); }
	@Test
	void testSetNumberNull() {
		Contact contact = new Contact("1234", "John", "Smith", "2104356672", "1234 Not Real Blvd.");
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			contact.setNumber(null);
		}); }
	@Test
	void testNumberTooLong() {
		Contact contact = new Contact("1234", "John", "Smith", "2104356672", "1234 Not Real Blvd.");
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			contact.setNumber("99999999999");
		}); }
	@Test
	void testSetAddressNull() {
		Contact contact = new Contact("1234", "John", "Smith", "2104356672", "1234 Not Real Blvd.");
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			contact.setAddress(null);
		}); }
	@Test
	void testSetAddressTooLong() {
		Contact contact = new Contact("1234", "John", "Smith", "2104356672", "1234 Not Real Blvd.");
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			contact.setAddress("1234567890 Very Long Not Real ");
		}); }
	}

}
