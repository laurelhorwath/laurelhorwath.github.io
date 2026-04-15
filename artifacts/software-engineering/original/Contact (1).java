package Contact;

public class Contact {
	private String id;
	private String firstName;
	private String lastName;
	private String number;
	private String address;
	
	public Contact(String id, String firstName, String lastName, String number, String address) {
		if (id == null) {
			throw new IllegalArgumentException("ID cannot be null");
		}
		if (id.length() > 10) {
			throw new IllegalArgumentException("ID cannot exceed 10 characters");
		}
		
		this.id = id;
		
		if (firstName == null) {
			throw new IllegalArgumentException("First name cannot be null");
		}
		if (firstName.length() > 10) {
			throw new IllegalArgumentException("First name cannot exceed 10 characters");
		}
		
		this.firstName = firstName;
		
		if (lastName == null) {
			throw new IllegalArgumentException("Last name cannot be null");
		}
		if (lastName.length() > 10) {
			throw new IllegalArgumentException("Last name cannot exceed 10 characters")
		}
		
		this.lastName = lastName;
		
		if (number == null) {
			throw new IllegalArgumentException("Phone number cannot be null");
		}
		if (number.length() > 10) {
			throw new IllegalArgumentException("Number cannot exceed 10 characters");
		}
		if (number.length() < 10) {
			throw new IllegalArgumentException("Number cannot be less than 10 characters")
		}
		
		this.number = number;
		
		if (address == null) {
			throw new IllegalArgumentException("AddresssAddress's cannot be null");
		}
		if (address.length() > 30) {
			throw new IllegalArgumentException("Address cannot exceed 30 characters");
		}
		
		this.address = address;
	}
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
	public void setAddress(String address) {
		if (address == null) {
			throw new IllegalArgumentException("Address cannot be null");
		}
		if (address.length() > 30) {
			throw new IllegalArgumentException("Address cannot exceed 30 characters");
		}
		this.address = address;
	}
	public void setFirstName(String firstName) {
		if (firstName == null) {
			throw new IllegalArgumentException("First name cannot be null");
		}
		if (firstName.length() > 10) {
			throw new IllegalArgumentException("First name cannot exceed 10 characters");
		}
		this.firstName = firstName;
	}
	public void setNumber(String number) {
		if (number == null) {
			throw new IllegalArgumentException("Number cannot be null");
		}
		if (number.length() > 10) {
			throw new IllegalArgumentException("Number cannot exceed 10 characters");
		}
		if (number.length() < 10) {
			throw new IllegalArgumentException("Number cannotcannot be less than 10 characters");
		}
		this.number = number;
	}
	public void setLastName(String lastName) {
		if (lastName == null) {
			throw new IllegalArgumentException("Last name cannot be null");
		}
		if (lastName.length() > 10) {
			throw new IllegalArgumentException("Last name cannot exceed 10 characters");
		}
		this.lastName = lastName;
	}
}

