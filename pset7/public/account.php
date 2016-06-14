<?php

     // configuration
    require("../includes/config.php"); 
    
    // Get all the necessary data from the db
    $user_id = $_SESSION["id"];
    $rows = CS50::query('SELECT cash FROM users WHERE id = ?', $user_id);
    $cash = $rows[0]["cash"];
    $formated_cash = number_format($cash, 2);
    // if user accessed page via GET
    if ($_SERVER["REQUEST_METHOD"] == "GET")
    {
        // Render forms
        render("account_view.php", ["title" => "Account", "cash" => $formated_cash]);
    }
    // else if user accessed page via POST
    elseif ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // Change the username
        if (!empty($_POST["new_username"]))
        {
            $username = $_POST["new_username"];
            // Change the username
            $new_name = CS50::query('UPDATE users SET username = ? WHERE id = ?', $username, $user_id);
            if ($new_name == 0)
            {
                apologize("Could not change the username!");
            }
            else
            {
                // redirect to the main page
                redirect("/");
            }
        }
        // Change password
        if (!empty($_POST["new_password"]))
        {
            if ($_POST["new_password"] !== $_POST["confirmation"])
            {
                apologize("You should confirm the new password!");
            }
            else
            {
                // Change the password
                $new_password = CS50::query('UPDATE users SET hash = ? WHERE id = ?', password_hash($_POST["new_password"], PASSWORD_DEFAULT), $user_id);
                if ($new_password == 0)
                {
                    apologize("Could not change the password!");
                }
                else
                {
                    // log out
                    logout();
                    // Redirect to the main page
                    redirect("/");
                }
            }
        }
        // Only password confirmation entered
        if (!empty($_POST["confirmation"]) && empty($_POST["new_password"]))
        {
            apologize("You should enter the new password as well!");
        }
        // Cash operations
        // Add cash
        if (!empty($_POST["add"]))
        {
            $money = $_POST["add"];
            if ($money <= 0)
            {
                apologize("Invalid cash operation!");
            }
            else if($money > 1000000)
            {
                apologize("That is too much for us!");
            }
            else
            {
                $new_cash = CS50::query('UPDATE users SET cash = cash + ? WHERE id = ?', $money, $user_id);
                if ($new_cash == 0)
                {
                    apologize("Could not adjust your account!");
                }
                else
                {
                    // Redirect to the main page
                    redirect("/");
                }
            }
        }
        // Withdraw cash
        if (!empty($_POST["withdraw"]))
        {
            $money = $_POST["withdraw"];
            if ($money <= 0)
            {
                apologize("Invalid cash operation!");
            }
            else if ($cash < $money)
            {
                apologize("You do not have enough cash!");
            }
            else
            {
                $new_cash = CS50::query('UPDATE users SET cash = cash - ? WHERE id = ?', $money, $user_id);
                if ($new_cash == 0)
                {
                    apologize("Could not adjust your account!");
                }
                else
                {
                    // Redirect to the main page
                    redirect("/");
                }
            }
        }
        // All $_POST variables are empty
        // Redirect to the account page
        redirect("/account.php");
    }

?>