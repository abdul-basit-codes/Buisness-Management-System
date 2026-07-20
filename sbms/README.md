# Small Business Management System (SBMS)

A C++ console application for managing a small service business —
customers, employees, services, bookings, and billing. Data is
persisted to plain CSV files in `data/`, so it needs no external
database.

## Features
- Customer management (add/view/edit/delete)
- Employee management (add/view/edit/delete)
- Service catalog management (add/view/edit/delete)
- Booking/appointment scheduling, with validation against existing customers/services/employees
- Invoice generation (subtotal, 5% tax, total) for any booking
- Business summary report (customer/employee/service/booking counts, completed revenue)
- Input validation on every form (no empty fields, no negative numbers)

## Build

```bash
make
./sbms
```

Or without make:

```bash
g++ -std=c++17 -Isrc src/*.cpp -o sbms
./sbms
```

## Project structure

```
src/       source files (Customer, Employee, Service, Booking, FileManager, main)
data/      persisted CSV data files (created at runtime, gitignored)
Makefile   convenience build target
```

## Sample workflow

1. Add a customer, an employee, and a service from their respective menus.
2. Create a booking that references their IDs.
3. Generate an invoice for that booking from the Billing menu.
4. Check the Reports menu for a running summary.

