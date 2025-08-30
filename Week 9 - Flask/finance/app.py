import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    id = session["user_id"]
    """Show portfolio of stocks"""
    portfolio = db.execute("""SELECT DISTINCT symbol AS "Stocks", SUM(number) AS "Number of shares"
                           FROM transactions WHERE userID = ?
                           GROUP BY symbol HAVING SUM(number) > 0""", id)
    for elt in portfolio:
        current_price = float(lookup(elt["Stocks"])["price"])
        elt["Current price"] = usd(current_price)
        elt["Total value"] = usd(current_price * elt["Number of shares"])

    cash = db.execute("SELECT cash FROM users WHERE id = ?", id)[0]['cash']
    total = db.execute(
        """SELECT SUM(number*price) AS "total" FROM transactions WHERE userID = ? """, id)[0]['total']
    total = total if total is not None else 0

    return render_template("index.html", portfolio=portfolio, cash=usd(cash), grand_total=usd(cash + total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")
    else:
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        try:
            shares = int(shares)
            if shares <= 0:
                raise ValueError("Shares must be a positive integer")
        except ValueError:
            return apology("Invalid number of shares", 400)
        if not symbol:
            return apology("must provide a symbol", 400)
        elif not lookup(symbol):
            return apology("No, that stock do not exist", 400)
        else:
            name = lookup(symbol)["symbol"]
            id = session["user_id"]
            price = lookup(symbol)["price"]
            cash = db.execute("SELECT cash FROM users WHERE id = ?", id)[0]['cash']
            if (cash < shares * price):
                return apology("Insufficient funds", 403)
            else:
                db.execute("""UPDATE users
                               SET cash = ?
                             WHERE id = ?""", cash - (shares * price), id)
                db.execute(
                    "INSERT INTO transactions (symbol, price, number, userID) VALUES(?, ?, ?, ?)", name, price, shares, id)
                return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    id = session["user_id"]
    transactions = db.execute(
        "SELECT symbol, price, number, date FROM transactions WHERE userID = ?", id)
    for transaction in transactions:
        transaction["price"] = usd(transaction["price"])
        if transaction["number"] > 0:
            transaction["type"] = "Purchase"
        else:
            transaction["number"] = - transaction["number"]
            transaction["type"] = "Sell"
    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")
    else:
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("must provide a symbol", 400)
        elif not lookup(symbol):
            return apology("No, that stock do not exist", 400)
        else:
            name = lookup(symbol)["symbol"]
            price = lookup(symbol)["price"]
            return render_template("quoted.html", name=name, price=usd(price))


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template("register.html")
    else:
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Ensure username was submitted
        if not username:
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not password:
            return apology("must provide password", 400)

       # Ensure password confirmation was submitted
        elif not confirmation:
            return apology("must confirm password", 400)

        elif password != confirmation:
            return apology("Passwords do not match", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)

        if len(rows) != 0:
            return apology("Username already exists", 400)
        else:
            new_user_id = db.execute(
                "INSERT INTO users (username, hash) VALUES(?, ?)", username, generate_password_hash(password))
            session["user_id"] = new_user_id
            return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    id = session["user_id"]
    if request.method == "GET":
        stocks = db.execute(
            "SELECT DISTINCT symbol FROM transactions WHERE userID = ? GROUP BY symbol HAVING SUM(number) > 0", id)
        return render_template("sell.html", stocks=stocks)
    else:
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        try:
            shares = int(shares)
            if shares <= 0:
                raise ValueError("Shares must be a positive integer")
        except ValueError:
            return apology("Invalid number of shares", 400)
        if not symbol:
            return apology("must provide a symbol", 403)
        else:
            number_shares = db.execute(
                """SELECT SUM(number) AS "Stock shares" FROM transactions WHERE userID = ? AND symbol=? """, id, symbol)[0]['Stock shares']
            if number_shares < shares:
                return apology("You don't own that many shares of the stock", 400)
            else:
                price = lookup(symbol)["price"]
                cash = db.execute("SELECT cash FROM users WHERE id = ?", id)[0]['cash']
                db.execute("""UPDATE users
                                SET cash = ?
                                WHERE id = ?""", cash + (shares * price), id)
                db.execute(
                    "INSERT INTO transactions (symbol, price, number, userID) VALUES(?, ?, ?, ?)", symbol, price, -shares, id)
                return redirect("/")


@app.route("/account")
@login_required
def settings():
    username = db.execute("SELECT username FROM users WHERE id = ?", session["user_id"])
    return render_template("account.html", username=username[0]['username'])


@app.route("/pwdupdate", methods=["GET", "POST"])
@login_required
def passwordupdate():
    """Show settings"""

    if request.method == "POST":

        # Validate submission
        currentpassword = request.form.get("currentpassword")
        newpassword = request.form.get("newpassword")
        confirmation = request.form.get("confirmation")

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])

        # Ensure password == confirmation
        if not (newpassword == confirmation):
            return apology("the passwords do not match", 400)

        # Ensure password not blank
        if currentpassword == "" or newpassword == "" or confirmation == "":
            return apology("input is blank", 400)

       # Ensure password is correct
        if not check_password_hash(rows[0]["hash"], currentpassword):
            return apology("invalid password", 403)
        else:
            hashcode = generate_password_hash(newpassword, method='pbkdf2:sha256', salt_length=8)
            db.execute("UPDATE users SET hash = ? WHERE id = ?", hashcode, session["user_id"])
        return redirect("/account")

    else:
        return render_template("pwdupdate.html")
