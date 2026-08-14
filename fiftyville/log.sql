-- Keep a log of any SQL queries you execute as you solve the mystery.

-- I did a .schema for this database and copied the output to a word doc so I could view all the tables and categories next to each other

-- From the instructions: You may find it helpful to start with the crime_scene_reports table.
-- Start by looking for a crime scene report that matches the date and the location of the crime.
SELECT description FROM crime_scene_reports
WHERE year = 2021 AND month = 7 AND day = 28;

-- RESULT: Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery.
-- Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery.


-- Do a search in the "interviews" table for "transcript" where bakery is mentioned
SELECT transcript FROM interviews
WHERE year = 2021 AND month = 7 AND day = 28
AND transcript LIKE "%bakery%";

-- RESULT:
-- 1) Sometime within ten minutes of the theft...If you have security footage from the bakery parking lot, you might want to look for cars
-- that left the parking lot in that time frame.
-- 2) Earlier this morning...I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.
-- 3) they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the
-- earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket.


-- 1) Do a search in the "bakery_security_logs" table for "license_plate" between 10:15-10:25 (within 10 minutes of the theft)
-- The "people" table also has a category of "license_plate", so i can get a name from joining on that
SELECT bakery_security_logs.activity, bakery_security_logs.license_plate, people.name FROM people
JOIN bakery_security_logs ON bakery_security_logs.license_plate = people.license_plate
WHERE bakery_security_logs.year = 2021 AND bakery_security_logs.month = 7 AND bakery_security_logs.day = 28
AND bakery_security_logs.hour = 10 AND bakery_security_logs.minute >= 15 AND bakery_security_logs.minute <=25;

-- RESULT: [Vanessa, Bruce, Barry, Luca, Sofia, Iman, Diana, Kelsey] exited during that time frame

-- 2) Do a search in the "atm_transactions" table for "transaction_type", "atm_location" is Leggett Street
-- The "bank_accounts" table also has a category of "account_number" that correlates with atm, and then
-- The "bank_accounts" table has a category of "person_id" that correlates with the "people" table, so i can get a name from joining on those
SELECT people.name, atm_transactions.transaction_type FROM people
JOIN bank_accounts ON bank_accounts.person_id = people.id
JOIN atm_transactions ON atm_transactions.account_number = bank_accounts.account_number
WHERE atm_transactions.year = 2021 AND atm_transactions.month = 7 AND atm_transactions.day = 28
AND atm_transactions.transaction_type = "withdraw"
AND atm_location = "Leggett Street";

-- RESULT: [_Bruce_, _Diana_, Brooke, Kenny, _Iman_, _Luca_, Taylor, Benista] exited during that time frame

-- 3) It sounds like at least 2 people are involved, and the "phone_calls" table lists both "caller" and "receiver".
-- The "people" table has a "phone_number" category, so i can search for the receiver to find the accomplice.
-- Then i can search for the people who were on that flight.

-- 3a phone calls (the call was less than a minute, 60 seconds)
SELECT caller, receiver FROM phone_calls
WHERE year = 2021 AND month = 7 AND day = 28
AND duration < 60;
-- Result: 10 sets of phone numbers (i copied this into my word doc)

-- getting caller name
SELECT people.name, phone_calls.caller FROM people
JOIN phone_calls ON phone_calls.caller = people.phone_number
WHERE year = 2021 AND month = 7 AND day = 28
AND duration < 60;
-- Result: [Sofia, Kelsey, _Bruce_, Kelsey, Taylor, _Diana_, Carina, Kenny, Benista] made calls less than a minute in duration

-- getting receiver name
SELECT people.name, phone_calls.receiver FROM people
JOIN phone_calls ON phone_calls.receiver = people.phone_number
WHERE year = 2021 AND month = 7 AND day = 28
AND duration < 60;
-- Result: [Bruce called Robin, Diana called Philip], the only two callers who match the other queries, as well


-- 3b plane tickets (earliest flight tomorrow (29th), receiver bought them)
SELECT id, hour, minute, origin_airport_id, destination_airport_id FROM flights
WHERE year = 2021 AND month = 7 AND day = 29;
-- Result: earliest flight is at 8:20, flight id = 36, from id 8 to id 4

-- get the name of the airport ids
SELECT destination_airport_id, full_name FROM airports
JOIN flights ON flights.destination_airport_id = airports.id
WHERE year = 2021 AND month = 7 AND day = 28;
-- Result: id 8 is Fiftyville, id 4 is LaGuardia (where they are going)

-- get the names of the people on the flight based on passport numbers
SELECT name FROM people
JOIN passengers ON passengers.passport_number = people.passport_number
WHERE flight_id = 36;
-- Result: Bruce is the only name that matches all other queries
