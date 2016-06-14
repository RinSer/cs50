<div>
    <table class="table table-striped table-hover table-responsive">
        <thead>
            <tr>
                <th>Transaction</th>
                <th>Date/Time</th>
                <th>Symbol</th>
                <th>Shares</th>
                <th>Price</th>
            </tr>
        </thead>
        <tbody>
        <?php foreach ($transactions as $transaction): ?>
            <tr>
                <td><?= $transaction["transaction"] ?></td>
                <td><?= $transaction["time"] ?></td>
                <td><?= $transaction["symbol"] ?></td>
                <td><?= $transaction["shares"] ?></td>
                <td>&#36;<?= number_format($transaction["price"], 2) ?></td>
            </tr>
        <?php endforeach ?>
        </tbody>
    </table>
</div>