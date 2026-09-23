import csv
import matplotlib.pyplot as plt

bids_p, bids_q, asks_p, asks_q = [], [], [], []

with open("book.csv") as f:
    for row in csv.DictReader(f):
        price = int(row["price"])
        qty = int(row["quantity"])
        side = row["side"]
        if(side == "bid"):
            bids_p.append(price)
            bids_q.append(qty)
        else:
            asks_p.append(price)
            asks_q.append(qty)

plt.bar(bids_p, bids_q, color="green", label="Bids")
plt.bar(asks_p, asks_q, color="red",   label="Asks")
plt.xlabel("Price"); plt.ylabel("Quantity")
plt.title("Order Book Depth")
plt.legend()
plt.savefig("depth.png", dpi=120)
print("saved depth.png")