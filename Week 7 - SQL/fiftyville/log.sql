-- Keep a log of any SQL queries you execute as you solve the mystery.

-- list all descriptions on the day of the theft
SELECT id, description FROM crime_scene_reports WHERE day = 28 AND month = 7 AND year = 2023 AND street = 'Humphrey Street';
/*The theft occurred at 10:15am as indicated in ID 295. The incident involved 3 witnesses and it's related to a bakery*/

-- list all interviews on the day of the theft
SELECT * FROM interviews WHERE day = 28 AND month = 7 AND year = 2023;

-- list all interviews on the day of the theft and related to the incident
SELECT transcript FROM interviews
 WHERE   day = 28 AND month = 7 AND year = 2023
         AND (id = 161 OR id = 162 OR id = 163);

/*The thief's car left the bakery's parking lot within 10-min timeframe of the theft.
The thief withdrew money from an ATM on Leggett Street.
The thief made a call lasting less than one minute. He is planning to take the earliest flight with his accomplice on 29/07/2023.*/

-- List the cars that left the bakery's parking lot within 10-min timeframe of the theft.
SELECT license_plate FROM bakery_security_logs
        WHERE  day = 28 AND month = 7 AND year = 2023 AND hour = 10
               AND (minute > 15 AND minute < 25);


-- list all calls lasting less than one minute on the day of the theft
SELECT caller, receiver FROM phone_calls
 WHERE duration <= 60
       AND day = 28 AND month = 7 AND year = 2023;

-- list the account numbers of individuals who withdrew money at Leggett Street on the day of the theft
SELECT person_id
       FROM bank_accounts
       JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
      WHERE transaction_type = "withdraw"
            AND atm_location = "Leggett Street"
            AND day = 28 AND month = 7 AND year = 2023;

-- narrowing suspected people

SELECT name, phone_number, passport_number
       FROM  people
      WHERE  id IN (SELECT person_id
                      FROM bank_accounts
                      JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
                     WHERE transaction_type = "withdraw"
                            AND atm_location = "Leggett Street"
                            AND day = 28 AND month = 7 AND year = 2023)
             AND phone_number IN (SELECT caller FROM phone_calls
                                   WHERE duration <= 60
                                         AND day = 28 AND month = 7 AND year = 2023)
             AND license_plate IN (SELECT license_plate FROM bakery_security_logs
                                    WHERE day = 28 AND month = 7 AND year = 2023 AND hour = 10
                                          AND minute > 15 AND minute < 25);
/*It's Diana 3592750733 or Bruce 5773159633 */

-- list the first flight departing from Fiftyville on the day following the theft

SELECT flights.id, destination_airport.city AS destination_city
       FROM  airports as origin_airport
             JOIN  flights ON origin_airport.id = flights.origin_airport_id
             JOIN  airports AS destination_airport ON flights.destination_airport_id = destination_airport.id
       WHERE origin_airport.city = "Fiftyville"
             AND day = 29 AND month = 7 AND year = 2023
    ORDER BY flights.hour
       LIMIT 1;

/* The thief took the flight with id = 36 to New York City*/

SELECT name, people.passport_number
       FROM  people
             JOIN passengers ON people.passport_number = passengers.passport_number
             WHERE flight_id IN (SELECT flights.id
                                    FROM  airports as origin_airport
                                          JOIN  flights ON origin_airport.id = flights.origin_airport_id
                                          JOIN  airports AS destination_airport ON flights.destination_airport_id = destination_airport.id
                                    WHERE origin_airport.city = "Fiftyville"
                                          AND day = 29 AND month = 7 AND year = 2023
                                  ORDER BY flights.hour
                                    LIMIT 1)
                   AND passengers.passport_number IN ( SELECT passport_number
                                                              FROM  people
                                                              WHERE  id IN (SELECT person_id
                                                                              FROM bank_accounts
                                                                              JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
                                                                            WHERE transaction_type = "withdraw"
                                                                                    AND atm_location = "Leggett Street"
                                                                                    AND day = 28 AND month = 7 AND year = 2023)
                                                                    AND phone_number IN (SELECT caller FROM phone_calls
                                                                                          WHERE duration <= 60
                                                                                                AND day = 28 AND month = 7 AND year = 2023)
                                                                    AND license_plate IN (SELECT license_plate FROM bakery_security_logs
                                                                                            WHERE day = 28 AND month = 7 AND year = 2023 AND hour = 10
                                                                                                  AND minute > 15 AND minute < 25));

/* It turns out that the thief is Bruce whose passport_number= 5773159633 */

SELECT name, passport_number, phone_number
  FROM people
 WHERE phone_number IN (SELECT receiver
                          FROM phone_calls
                               JOIN people on phone_calls.caller = people.phone_number
                              WHERE passport_number = 5773159633
                                    AND duration < 60
                                    AND day = 28 AND month = 7 AND year = 2023);
