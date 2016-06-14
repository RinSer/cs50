<?php

    // configuration
    require("../includes/config.php"); 
    
    // Get all the necessary data from the db
    $id = $_SESSION["id"];
    $rows = CS50::query('SELECT symbol, shares FROM portfolios WHERE user_id = ?', $id);
    // Populate the positions array
    $positions = [];
    foreach ($rows as $row)
    {
        $stock = lookup($row["symbol"]);
        if ($stock !== false)
        {
            $positions[] = [
                    "symbol" => $stock["symbol"],
                    "name" => $stock["name"],
                    "shares" => $row["shares"],
                    "price" => $stock["price"],
                    "total" => $stock["price"]*$row["shares"]
                ];
        }
    }
    
    // Get the user cash
    $cash = number_format(CS50::query('SELECT cash FROM users WHERE id = ?', $id)[0]["cash"], 2);

    // render portfolio
    render("portfolio.php", ["title" => "Portfolio", "positions" => $positions, "cash" => $cash]);

?>
