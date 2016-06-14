<?php

    // configuration
    require("../includes/config.php");

    // if user reached page via GET (as by clicking a link or via redirect)
    if ($_SERVER["REQUEST_METHOD"] == "GET")
    {
        // else render form
        render("register_form.php", ["title" => "Register"]);
    }

    // else if user reached page via POST (as by submitting a form via POST)
    else if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // validate submissions
        if (empty($_POST["username"]))
        {
            apologize("You must provide a unique username!");
        }
        elseif (empty($_POST["password"]))
        {
            apologize("You must provide a password!");
        }
        elseif ($_POST["password"] != $_POST["confirmation"])
        {
            apologize("You must confirm your password!");
        }
        else
        {
            // Store the query function return value into a variable
            $registered = CS50::query("INSERT IGNORE INTO users (username, hash, cash) VALUES(?, ?, 10000.0000)", $_POST["username"], password_hash($_POST["password"], PASSWORD_DEFAULT));
            if ($registered == 0)
            {
                apologize("Try to choose another username.");
            }
            else
            {
                $rows = CS50::query("SELECT LAST_INSERT_ID() AS id");
                // Initiate new session with the new user's id
                $_SESSION["id"] = $rows[0]["id"];
                // Redirect the new user to his portfolio
                redirect("/");
            }
        }
    }

?>