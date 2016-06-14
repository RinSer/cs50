<h2>Change the username</h2>
<form action="account.php" method="post">
    <fieldset>
        <div class="form-group">
            <input autocomplete="off" autofocus class="form-control" name="new_username" placeholder="New Username" type="text"/>
        </div>
        <div class="form-group">
            <button class="btn btn-default" type="submit">
                Change the username
            </button>
        </div>
    </fieldset>
</form>
<h2>Change the password</h2>
<form action="account.php" method="post">
    <fieldset>
        <div class="form-group">
            <input class="form-control" name="new_password" placeholder="New Password" type="password"/>
        </div>
        <div class="form-group">
            <input class="form-control" name="confirmation" placeholder="Password Confirmation" type="password"/>
        </div>
        <div class="form-group">
            <button class="btn btn-default" type="submit">
                Change the password
            </button>
        </div>
    </fieldset>
</form>
<h2>Cash operations</h2>
<form action="account.php" method="post">
    <fieldset>
        <div class="form-group">
            <input class="form-control" name="add" type="number" step="0.01" placeholder="&#36;<?= $cash ?>" />
        </div>
        <div class="form-group">
            <button class="btn btn-default" type="submit">
                Add cash
            </button>
        </div>
    </fieldset>
</form>
<form action="account.php" method="post">
    <fieldset>
        <div class="form-group">
            <input class="form-control" name="withdraw" type="number" step="0.01" placeholder="&#36;<?= $cash ?>" />
        </div>
        <div class="form-group">
            <button class="btn btn-default" type="submit">
                Withdraw
            </button>
        </div>
    </fieldset>
</form>

