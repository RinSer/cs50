<div>
    <h2>Your shares</h2>
    <table class="table table-striped table-hover table-responsive">
        <thead>
            <tr>
                <th>Symbol</th>
                <th>Name</th>
                <th>Shares</th>
                <th>Price</th>
                <th>Total</th>
            </tr>
        </thead>
        <tbody>
        <?php foreach ($positions as $position): ?>
            <tr>
                <td><?= $position["symbol"] ?></td>
                <td><?= $position["name"] ?></td>
                <td><?= $position["shares"] ?></td>
                <td><?= $position["price"] ?></td>
                <td><?= $position["total"] ?></td>
            </tr>
        <?php endforeach ?>
            <tr>
                <td>Cash</td>
                <td></td>
                <td></td>
                <td></td>
                <td><?= $cash ?></td>
            </tr>
        </tbody>
    </table>
</div>