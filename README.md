# CP-077 RationRight
## Entitlement-Based Public Distribution Tracker

RationRight is a menu-driven console application written in C. It helps a fair-price shop register families, calculate monthly ration entitlements, record distribution, manage stock, and prepare collection reports.

## Features

1. **Family registration**
   - Register a family using a unique family ID.
   - Store the family head's name, ration card type (A, B, or C), and number of members.
   - Reject duplicate family IDs.

2. **Monthly entitlement calculation**
   - Calculate rice, wheat, and sugar entitlement using the selected card type.
   - Multiply the per-member quantity by the number of family members.

3. **Ration distribution**
   - Record a family's collection for a specified month (`YYYY-MM`).
   - Prevent a family from collecting a second time in the same month.
   - Deduct distributed quantities from the shop's stock.

4. **Stock management**
   - View current rice, wheat, and sugar stock.
   - Add new stock.
   - Refuse a distribution if any required item has insufficient stock.

5. **Reports**
   - List families who have not collected for a selected month.
   - Show the number of pending families.
   - Show remaining shop stock.

Additional menu options list registered families and exit the program.

## Requirements

- A C compiler such as GCC (or an IDE with a C compiler).
- A terminal or command prompt.

## Files

- `rationright.c` — complete C source code.
- `README.md` — project documentation.

## How to compile and run

### Linux / macOS

```bash
gcc -std=c11 -Wall -Wextra -pedantic rationright.c -o rationright
./rationright
```

### Windows (MinGW GCC)

```bash
gcc -std=c11 -Wall -Wextra -pedantic rationright.c -o rationright.exe
rationright.exe
```

You can also open `rationright.c` in Code::Blocks, VS Code, or another C IDE, build the program, and run it.

## First-time setup: entitlement rates

At startup, the program asks for the monthly quantity **per family member** for each card type:

- Card A: rice, wheat, and sugar
- Card B: rice, wheat, and sugar
- Card C: rice, wheat, and sugar

Enter the rates supplied or approved by your trainer. The project brief does not provide exact quantities or define the card categories, so this program deliberately does not assume official ration rates.

Use one consistent unit for all quantities (for example, kilograms).

## How to use the menu

1. Start the program and enter the approved entitlement rates.
2. Choose **1** to register families.
3. Choose **2** to calculate a family's monthly entitlement.
4. Choose **4** to add the shop's opening stock.
5. Choose **3** to distribute ration to a registered family.
6. Choose **5** to view pending families and remaining stock.
7. Choose **6** to view all registered families.
8. Choose **7** to exit.

**Important:** Add sufficient stock before trying to distribute ration.

## Example calculation

Suppose a trainer-approved Card A rate is:

- Rice: 5 kg per member
- Wheat: 2 kg per member
- Sugar: 1 kg per member

For a family with 4 members, the program calculates:

- Rice: 5 × 4 = 20 kg
- Wheat: 2 × 4 = 8 kg
- Sugar: 1 × 4 = 4 kg

These numbers are only an example to explain the calculation. They are not presented as official ration rates.

## Input validation and business rules

- Family ID must not be blank or already registered.
- Family name must not be blank.
- Card type must be A, B, or C.
- Family size must be between 1 and 50.
- Stock additions and entitlement quantities cannot be negative.
- Month must use `YYYY-MM` format with a valid month from `01` to `12`.
- Distribution is blocked when the family has already collected in the selected month.
- Distribution is blocked when stock is insufficient.
- Stock is reduced and the collection month is recorded only after a successful distribution.

## Testing checklist

- Register a new family.
- Try to register the same family ID again.
- Calculate entitlements for A, B, and C card types.
- Add stock and complete a distribution.
- Try to distribute to the same family again in the same month.
- Try a distribution when one or more stock items are insufficient.
- Check the pending-family report.
- Check that stock decreases correctly after distribution.
- Enter invalid menu choices, invalid card types, and invalid month formats.

## Current limitations

- **In-memory storage:** Family records, stock, and collection records are stored in program memory. They reset when the application exits.
- **One collection record per family:** This version records the month of the latest successful collection. It is designed for normal month-by-month operation; it does not maintain a permanent transaction history.
- **No file/database storage:** Persistent text/CSV storage is an optional extension and is not included in this basic version.
- **Trainer-approved rules:** Actual card categories and entitlement rates must be confirmed with the trainer or the applicable project specification.

## Possible stretch features

- Save and load family and stock data using a text or CSV file.
- Maintain a full distribution transaction history.
- Support partial collection and carry forward the remaining entitlement within the month.
- Flag unusual distribution patterns.

## Viva preparation

Be ready to explain:

- `struct` and why it is used for family and stock records.
- Arrays and how family records are stored.
- Functions and modular program design.
- `switch-case` and the `do-while` menu loop.
- How monthly entitlement is calculated.
- How duplicate collection is prevented.
- How the stock sufficiency check works.
- Why validation is important.

## Author / team

Add your name, partner's name (if any), class, and date before submitting.

---

**Project:** CP-077 RationRight  
**Language:** C  
**Application type:** Menu-driven console application
