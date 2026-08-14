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
    """Show portfolio of stocks"""
    # Complete the implementation of index in such a way that it displays an HTML table summarizing,
    # for the user currently logged in, which stocks the user owns, the numbers of shares owned, the
    # current price of each stock, and the total value of each holding (i.e., shares times price).
    # Also display the user’s current cash balance along with a grand total (i.e., stocks’ total value plus cash).

    # Odds are you’ll want to execute multiple SELECTs. Depending on how you implement your table(s),
    # you might find GROUP BY HAVING SUM and/or WHERE of interest.
    stocks = db.execute("SELECT symbol, SUM(shares) as totalshares FROM brandontable WHERE user_id = :user_id GROUP BY symbol HAVING totalshares > 0", user_id=session["user_id"])
    cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])[0]["cash"]
    stockvalue = 0

    # Odds are you’ll want to call lookup for each stock.
    for stock in stocks:
        quote = lookup(stock["symbol"])
        stock["price"] = quote["price"]
        stock["value"] = (stock["price"] * stock["totalshares"])
        stockvalue += stock["value"]

    # feeding vars into the html
    return render_template("index.html", stocks=stocks, cash=cash, stockvalue=stockvalue)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        # Require that a user input a stock’s symbol, implemented as a text field whose name is symbol. Render an apology if the input is blank
        if not symbol:
            return apology("symbol cannot be blank")
        # Require that a user input a number of shares, implemented as a text field whose name is shares. Render an apology if the input is not a positive integer.
        elif not shares or int(shares) <= 0:
            return apology("shares must be more than 0")

        # Render an apology if the symbol does not exist (as per the return value of lookup).
        # Odds are you’ll want to call lookup to look up a stock’s current price.
        quote = lookup(symbol)
        if quote is None:
            return apology("symbol does not exist")

        price = quote["price"]
        totalcost = int(shares) * price
        # Odds are you’ll want to SELECT how much cash the user currently has in users.
        cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])[0]["cash"]

        # Render an apology, without completing a purchase, if the user cannot afford the number of shares at the current price.
        if cash < totalcost:
            return apology("not enough cash")


        # Add one or more new tables to finance.db via which to keep track of the purchase. Store enough information so that you know who bought what at what price and when.
        # Update the user's cash amount
        db.execute("UPDATE users SET cash = cash - :totalcost WHERE id = :user_id", totalcost=totalcost, user_id=session["user_id"])

        # Update the user's purchase history
        db.execute("INSERT INTO brandontable (user_id, symbol, shares, price) VALUES (:user_id, :symbol, :shares, :price)", user_id=session["user_id"], symbol=symbol, shares=shares, price=price)


        # Upon completion, redirect the user to the home page.
        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # Complete the implementation of history in such a way that it displays an HTML table summarizing
    # all of a user’s transactions ever, listing row by row each and every buy and every sell.

    # For each row, make clear whether a stock was bought or sold and include the stock’s symbol,
    # the (purchase or sale) price, the number of shares bought or sold, and the date and time at which the transaction occurred.

    # You might need to alter the table you created for buy or supplement it with an additional table. Try to minimize redundancies.

    transactions = db.execute("SELECT * FROM brandontable WHERE user_id = :user_id ORDER BY timestamp DESC", user_id=session["user_id"])

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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
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
    # When a user visits /quote via GET, render one of those templates, inside of which should be an HTML form that submits to /quote via POST.
    if request.method == "GET":
        return render_template("quote.html")

    # In response to a POST, quote can render that second template, embedding within it one or more values from lookup.
    else:
        symbol = request.form.get("symbol")
        quote = lookup(symbol)
        if not quote:
            return apology("Invalid Stock Ticker Symbol")
        return render_template("quoted.html", quote=quote)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    session.clear()

    if request.method == "POST":
        # Render an apology if the user’s input is blank
        if not request.form.get("username"):
            return apology("No username entered!")

        #  or the username already exists.
        dbnames = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))
        if len(dbnames) !=0:
            return apology("Username already exists!")

        # Render an apology if either [password] input is blank or the passwords do not match.
        if not request.form.get("password"):
            return apology("Password cannot be blank!")

        elif not request.form.get("passconfirm"):
            return apology("Confirmation cannot be blank!")

        elif request.form.get("password") != request.form.get("passconfirm"):
            return apology("Passwords do not match!")

        # INSERT the new user into users, storing a hash of the user’s password, not the password itself.
        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", request.form.get("username"), generate_password_hash(request.form.get("password")))

        return redirect("/")

    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    stocks = db.execute("SELECT symbol, SUM(shares) as totalshares FROM brandontable WHERE user_id = :user_id GROUP BY symbol HAVING totalshares > 0", user_id=session["user_id"])

    #Submit the user’s input via POST to /sell.
    if request.method == "POST":
        # Require that a user input a stock’s symbol, implemented as a select menu whose name is symbol.
        # Require that a user input a number of shares, implemented as a text field whose name is shares.
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # Render an apology if the user fails to select a stock or if (somehow, once submitted) the user
        # does not own any shares of that stock.
        if not symbol:
            return apology("no symbol entered")
        # Render an apology if the input is not a positive integer or if the user does not own that many
        # shares of the stock.
        elif not shares or int(shares) <= 0:
            return apology("shares must be greater than 0")

        else:
            shares = int(shares)

        for stock in stocks:
            if stock["symbol"] == symbol:
                if stock ["totalshares"] < shares:
                    return apology("you don't own that many shares")
                else:
                    quote = lookup(symbol)
                    if quote is None:
                        return apology("symbol not found")
                    price = quote["price"]
                    totalsellprice = shares * price

                    #-- this updates the user database with user's new cash total after selling
                    db.execute("UPDATE users SET cash = cash + :totalsellprice WHERE id = :user_id", totalsellprice=totalsellprice, user_id=session["user_id"])

                    #-- this updates my created table in the database after selling, adding to the "history" of the user's transactions (ie: -10 shares)
                    db.execute("INSERT INTO brandontable (user_id, symbol, shares, price) VALUES (:user_id, :symbol, :shares, :price)", user_id=session["user_id"], symbol=symbol, shares=-shares, price=price)

                    # Upon completion, redirect the user to the home page.
                    return redirect("/")

        return apology("symbol not found")

    # if the page is loaded by GET instead of POST
    else:
        return render_template("sell.html", stocks=stocks)

# adding personal flair here to enable a deposit
@app.route("/deposit", methods=["POST"])
@login_required
def deposit():
    if request.method == "POST":
        deposit = request.form.get("deposit")

        if not deposit or int(deposit) <= 0:
            # don't allow negative or blank deposits, ruins the table
            return apology("deposit must be positive")
        else:
            db.execute("UPDATE users SET cash = cash + :deposit WHERE id = :user_id", deposit=deposit, user_id=session["user_id"])

            # update the history with deposit record
            # "0" value for shares messes up the "total" column for deposits, but leaving it blank causes
            # a problem with usd and strings, and using "1" causes and issue with the index page showing deposits as stock holdings
            db.execute("INSERT INTO brandontable (user_id, symbol, shares, price) VALUES (:user_id, :symbol, :shares, :price)", user_id=session["user_id"], symbol="DEPOSIT", shares="0", price=deposit)

            return redirect("/")

    else:
        return render_template("index.html", stocks=stocks, cash=cash, stockvalue=stockvalue)

