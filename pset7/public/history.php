<?php

    // configuration
    require("../includes/config.php"); 
    
    // Get all the necessary data from the db
    $user_id = $_SESSION["id"];
    $rows = CS50::query('SELECT * FROM history WHERE user_id = ?', $user_id);
    $transactions = [];
    foreach ($rows as $row)
    {
        $transactions[] = [
                "time" => $row["time"],
                "transaction" => $row["transaction"],
                "symbol" => $row["symbol"],
                "shares" => $row["shares"],
                "price" => $row["price"]
            ];
    }
    if (empty($transactions))
    {
        apologize("You have not done anything yet!");
    }
    else
    {
        // Render the history view
        render("history_view.php", ["title" => "History", "transactions" => $transactions]);
    }

?>