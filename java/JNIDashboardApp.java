import javafx.application.Application;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.layout.*;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.scene.shape.Line;
import javafx.scene.text.Font;
import javafx.scene.text.FontWeight;
import javafx.stage.Stage;

import java.util.LinkedHashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class JNIDashboardApp extends Application {

    private final EcashAPI api = new EcashAPI();

    private Label totalCoinsLabel;
    private Label availableStatusLabel;
    private Label systemStatusLabel;
    private Label currentPhaseLabel;
    private Label explanationLabel;

    private VBox actionBox;
    private TextArea logArea;
    private GridPane valueGrid;
    private HBox timelineBox;

    private int totalCoins = 0;
    private boolean coinSpent = false;

    private static final String DARK = "#111827";
    private static final String BLUE = "#2563eb";
    private static final String GREEN = "#059669";
    private static final String RED = "#dc2626";
    private static final String BG = "#f4f7fb";

    @Override
    public void start(Stage stage) {
        BorderPane root = new BorderPane();
        root.setPadding(new Insets(18));
        root.setStyle("-fx-background-color: " + BG + ";");

        root.setTop(buildHeader());
        root.setCenter(buildContent());

        Scene scene = new Scene(root, 1360, 820);
        stage.setTitle("Provably Secure E-Cash Scheme - Interactive Demo");
        stage.setScene(scene);
        stage.show();

        runAction("Initialize System", "System Setup", api::initSystem);
    }

    private VBox buildHeader() {
        VBox wrapper = new VBox(12);
        wrapper.setPadding(new Insets(18));
        wrapper.setStyle(card("#ffffff") + border(DARK, 3));

        HBox titleRow = new HBox(16);
        titleRow.setAlignment(Pos.CENTER_LEFT);

        VBox titleBox = new VBox(5);
        Label title = new Label("Provably Secure E-Cash Scheme");
        title.setFont(Font.font("Arial", FontWeight.BOLD, 28));
        title.setTextFill(Color.web(DARK));

        Label subtitle = new Label("Interactive JNI Dashboard · User Wallet · Bank Authority · Merchant Verifier");
        subtitle.setFont(Font.font("Arial", 15));
        subtitle.setTextFill(Color.web("#4b5563"));
        titleBox.getChildren().addAll(title, subtitle);

        Region spacer = new Region();
        HBox.setHgrow(spacer, Priority.ALWAYS);

        systemStatusLabel = chip("System: Starting", "#fef3c7", "#92400e");
        currentPhaseLabel = chip("Phase: Initialization", "#dbeafe", "#1d4ed8");

        titleRow.getChildren().addAll(titleBox, spacer, systemStatusLabel, currentPhaseLabel);

        HBox stats = new HBox(12);
        stats.setAlignment(Pos.CENTER_LEFT);
        totalCoinsLabel = statCard("Total Coins", "0");
        availableStatusLabel = statCard("Wallet State", "No Coin Yet");
        Label backend = statCard("Backend", "C++ + JNI + AMCL");
        Label security = statCard("Security Focus", "BBS+ · Commitment · Serial Check");
        stats.getChildren().addAll(totalCoinsLabel, availableStatusLabel, backend, security);

        wrapper.getChildren().addAll(titleRow, stats, buildTimeline());
        return wrapper;
    }

    private HBox buildTimeline() {
        timelineBox = new HBox(10);
        timelineBox.setAlignment(Pos.CENTER_LEFT);
        timelineBox.setPadding(new Insets(8, 0, 0, 0));
        setTimeline("System Setup");
        return timelineBox;
    }

    private void setTimeline(String active) {
        if (timelineBox == null) return;
        timelineBox.getChildren().clear();
        String[] phases = {"System Setup", "Withdraw", "View Wallet", "Spend", "Verify / Deposit", "Double-Spend Demo"};
        for (int i = 0; i < phases.length; i++) {
            Label step = new Label((i + 1) + ". " + phases[i]);
            boolean isActive = phases[i].equals(active);
            step.setStyle("-fx-background-color: " + (isActive ? DARK : "#e5e7eb") + ";"
                    + "-fx-text-fill: " + (isActive ? "white" : DARK) + ";"
                    + "-fx-padding: 8 12; -fx-background-radius: 18; -fx-font-weight: bold;");
            timelineBox.getChildren().add(step);
        }
    }

    private HBox buildContent() {
        HBox content = new HBox(18);
        content.setPadding(new Insets(18, 0, 0, 0));

        VBox left = new VBox(18, buildArchitecturePanel(), buildActionPanel());
        left.setPrefWidth(480);
        left.setMinWidth(480);

        VBox right = new VBox(18, buildExplanationPanel(), buildValuePanel());
        HBox.setHgrow(right, Priority.ALWAYS);

        content.getChildren().addAll(left, right);
        return content;
    }

    private StackPane buildArchitecturePanel() {
        StackPane card = new StackPane();
        card.setPrefHeight(390);
        card.setStyle(card("#eef2ff") + border(DARK, 3));

        Pane canvas = new Pane();
        canvas.setPrefSize(480, 390);

        VBox user = roleNode("USER", "Wallet", "Withdraws & spends coin", "#dbeafe", "#1d4ed8");
        user.setLayoutX(170);
        user.setLayoutY(25);
        user.setOnMouseClicked(e -> showUserActions());

        VBox bank = roleNode("BANK", "Issuer", "Signs & checks serial", "#dcfce7", "#047857");
        bank.setLayoutX(28);
        bank.setLayoutY(230);
        bank.setOnMouseClicked(e -> showBankActions());

        VBox merchant = roleNode("MERCHANT", "Verifier", "Verifies payment proof", "#fce7f3", "#be185d");
        merchant.setLayoutX(310);
        merchant.setLayoutY(230);
        merchant.setOnMouseClicked(e -> showMerchantActions());

        Line l1 = line(238, 145, 115, 242);
        Line l2 = line(245, 145, 365, 242);
        Line l3 = line(150, 285, 315, 285);

        Label withdraw = flowTag("Withdraw / Issue", 55, 170, BLUE);
        Label spend = flowTag("Spend Coin", 330, 170, "#be185d");
        Label deposit = flowTag("Verify + Deposit", 180, 300, GREEN);

        canvas.getChildren().addAll(l1, l2, l3, withdraw, spend, deposit, user, bank, merchant);
        card.getChildren().add(canvas);
        return card;
    }

    private VBox buildActionPanel() {
        VBox panel = new VBox(14);
        panel.setPadding(new Insets(18));
        panel.setStyle(card("#ffffff") + border(DARK, 3));

        Label title = new Label("Protocol Control Panel");
        title.setFont(Font.font("Arial", FontWeight.BOLD, 22));

        Label hint = new Label("Click a role in the architecture diagram or use the buttons below to execute real C++ backend functions through JNI.");
        hint.setWrapText(true);
        hint.setTextFill(Color.web("#4b5563"));

        actionBox = new VBox(10);
        showUserActions();

        logArea = new TextArea();
        logArea.setEditable(false);
        logArea.setWrapText(true);
        logArea.setPrefHeight(165);
        logArea.setStyle("-fx-font-family: Consolas; -fx-font-size: 12px; -fx-control-inner-background: #f9fafb;");

        panel.getChildren().addAll(title, hint, actionBox, new Label("Latest Backend Response"), logArea);
        return panel;
    }

    private VBox buildExplanationPanel() {
        VBox panel = new VBox(8);
        panel.setPadding(new Insets(18));
        panel.setStyle(card("#ffffff") + border("#d1d5db", 2));

        Label title = new Label("Current Protocol Explanation");
        title.setFont(Font.font("Arial", FontWeight.BOLD, 22));

        explanationLabel = new Label("The system starts by initializing the wallet, bank, and merchant objects. Each later action returns real values from the C++ backend.");
        explanationLabel.setWrapText(true);
        explanationLabel.setFont(Font.font("Arial", 15));
        explanationLabel.setTextFill(Color.web("#374151"));

        panel.getChildren().addAll(title, explanationLabel);
        return panel;
    }

    private VBox buildValuePanel() {
        VBox panel = new VBox(12);
        panel.setPadding(new Insets(18));
        panel.setStyle(card("#ffffff") + border(DARK, 3));

        Label title = new Label("Significant Cryptographic Values");
        title.setFont(Font.font("Arial", FontWeight.BOLD, 23));

        Label subtitle = new Label("These values prove the UI is backed by the real C++ protocol, not static mock data.");
        subtitle.setTextFill(Color.web("#4b5563"));

        valueGrid = new GridPane();
        valueGrid.setHgap(8);
        valueGrid.setVgap(8);
        valueGrid.setPadding(new Insets(10));

        ScrollPane scroll = new ScrollPane(valueGrid);
        scroll.setFitToWidth(true);
        scroll.setPrefHeight(490);
        scroll.setStyle("-fx-background: #ffffff; -fx-background-color: #ffffff;");

        panel.getChildren().addAll(title, subtitle, scroll);
        VBox.setVgrow(scroll, Priority.ALWAYS);
        return panel;
    }

    private void showUserActions() {
        actionBox.getChildren().setAll(
                actionButton("1. View Wallet", () -> runAction("View Wallet", "View Wallet", api::viewWallet,
                        "The wallet shows the stored coin count and the latest coin values, including serial number, randomness, commitment, and signature.")),
                actionButton("2. Spend Coin", () -> runAction("Spend Coin", "Spend", api::spendCoin,
                        "The user spends a coin by submitting values to the merchant. The merchant verifies the coin without receiving the user's identity."))
        );
    }

    private void showBankActions() {
        actionBox.getChildren().setAll(
                actionButton("1. Withdraw / Issue Coin", () -> runAction("Withdraw Coin", "Withdraw", api::withdrawCoin,
                        "The bank issues a signed coin to the user. The output shows the serial number, randomness, commitment, blind factor, and BBS+ signature values.")),
                actionButton("2. Double-Spending Demo", () -> runAction("Double-Spending Demo", "Double-Spend Demo", api::doubleSpendDemo,
                        "The same coin is deposited twice. The first deposit is accepted, while the second is rejected because the serial number already exists."))
        );
    }

    private void showMerchantActions() {
        actionBox.getChildren().setAll(
                actionButton("1. Verify / Deposit Payment", () -> runAction("Verify Payment", "Verify / Deposit", api::verifyPayment,
                        "The merchant verifies the coin, then the bank checks the serial number and records the coin as spent.")),
                actionButton("2. View Wallet After Deposit", () -> runAction("View Wallet", "View Wallet", api::viewWallet,
                        "After a successful deposit, the wallet should show that the latest coin has been marked as spent."))
        );
    }

    private Button actionButton(String text, Runnable action) {
        Button btn = new Button(text);
        btn.setMaxWidth(Double.MAX_VALUE);
        btn.setMinHeight(45);
        btn.setStyle("-fx-background-color: " + DARK + "; -fx-text-fill: white; -fx-font-weight: bold; -fx-font-size: 13px; -fx-background-radius: 10;");
        btn.setOnAction(e -> action.run());
        return btn;
    }

    private void runAction(String name, String phase, JNIAction action, String explanation) {
        try {
            String json = action.run();
            Map<String, String> values = parseJson(json);
            displayValues(values);
            logArea.setText("[" + name + "]\n" + json);
            setTimeline(phase);
            currentPhaseLabel.setText("Phase: " + phase);
            explanationLabel.setText(explanation);

            boolean success = "true".equals(values.get("success"));
            systemStatusLabel.setText(success ? "System: OK" : "System: Error");
            systemStatusLabel.setStyle(chipStyle(success ? "#dcfce7" : "#fee2e2", success ? "#047857" : RED));

            if (values.containsKey("totalCoins")) {
                totalCoins = safeInt(values.get("totalCoins"));
                totalCoinsLabel.setText("Total Coins\n" + totalCoins);
            }

            if (values.containsKey("latestSpent")) {
                coinSpent = "true".equals(values.get("latestSpent"));
                availableStatusLabel.setText("Wallet State\n" + (coinSpent ? "Latest Coin Spent" : "Coin Available"));
            }

            if (values.containsKey("spentAfterDeposit") && "true".equals(values.get("spentAfterDeposit"))) {
                coinSpent = true;
                availableStatusLabel.setText("Wallet State\nLatest Coin Spent");
            }

            if (values.containsKey("doubleSpendDetected")) {
                boolean detected = "true".equals(values.get("doubleSpendDetected"));
                availableStatusLabel.setText("Attack Demo\n" + (detected ? "Double-Spend Detected" : "Not Detected"));
            }

        } catch (Throwable ex) {
            logArea.setText("[" + name + "]\nERROR: " + ex.getMessage());
            systemStatusLabel.setText("System: Error");
            systemStatusLabel.setStyle(chipStyle("#fee2e2", RED));
        }
    }

    private void runAction(String name, String phase, JNIAction action) {
        runAction(name, phase, action, "The selected backend operation has been executed through JNI.");
    }

    private void displayValues(Map<String, String> values) {
        valueGrid.getChildren().clear();
        int row = 0;

        String[] priority = {
                "success", "message", "serial", "latestSerial", "spendSerial", "serialChecked",
                "randomness", "latestRandomness", "value", "blindFactor", "latestBlindFactor",
                "commitment", "latestCommitment", "blindedCommitment", "latestBlindedCommitment",
                "signatureA", "latestSignatureA", "signatureE", "latestSignatureE", "signatureS", "latestSignatureS",
                "merchantVerified", "bankAccepted", "spentAfterDeposit", "latestSpent",
                "firstDepositAccepted", "secondDepositAccepted", "doubleSpendDetected", "explanation"
        };

        for (String key : priority) {
            if (values.containsKey(key)) {
                addValueRow(row++, key, values.get(key));
            }
        }

        for (Map.Entry<String, String> entry : values.entrySet()) {
            boolean alreadyShown = false;
            for (String key : priority) {
                if (key.equals(entry.getKey())) {
                    alreadyShown = true;
                    break;
                }
            }
            if (!alreadyShown) addValueRow(row++, entry.getKey(), entry.getValue());
        }
    }

    private void addValueRow(int row, String keyText, String valueText) {
        Label key = new Label(formatKey(keyText));
        key.setMinWidth(170);
        key.setFont(Font.font("Arial", FontWeight.BOLD, 12));
        key.setStyle("-fx-background-color: #f3f4f6; -fx-padding: 9; -fx-border-color: #e5e7eb;");

        TextArea val = new TextArea(valueText == null ? "" : valueText);
        val.setEditable(false);
        val.setWrapText(true);
        val.setPrefRowCount(valueText != null && valueText.length() > 100 ? 3 : 1);
        val.setStyle("-fx-font-family: Consolas; -fx-font-size: 11px; -fx-control-inner-background: white;");

        valueGrid.add(key, 0, row);
        valueGrid.add(val, 1, row);
        GridPane.setHgrow(val, Priority.ALWAYS);
    }

    private Map<String, String> parseJson(String json) {
        Map<String, String> map = new LinkedHashMap<>();
        Pattern p = Pattern.compile("\\\"([^\\\"]+)\\\"\\s*:\\s*(\\\"([^\\\"]*)\\\"|true|false|-?\\d+)");
        Matcher m = p.matcher(json);
        while (m.find()) {
            String key = m.group(1);
            String raw = m.group(2);
            String value = raw.startsWith("\"") ? m.group(3) : raw;
            map.put(key, value);
        }
        return map;
    }

    private int safeInt(String text) {
        try { return Integer.parseInt(text); } catch (Exception e) { return 0; }
    }

    private String formatKey(String key) {
        String spaced = key.replaceAll("([A-Z])", " $1");
        return spaced.substring(0, 1).toUpperCase() + spaced.substring(1);
    }

    private VBox roleNode(String title, String role, String desc, String bg, String accent) {
        Circle c = new Circle(52);
        c.setFill(Color.web(bg));
        c.setStroke(Color.web(accent));
        c.setStrokeWidth(5);

        Label t = new Label(title);
        t.setFont(Font.font("Arial", FontWeight.BOLD, 16));
        Label r = new Label(role);
        r.setFont(Font.font("Arial", FontWeight.BOLD, 12));
        r.setTextFill(Color.web(accent));
        Label d = new Label(desc);
        d.setWrapText(true);
        d.setMaxWidth(135);
        d.setAlignment(Pos.CENTER);
        d.setTextFill(Color.web("#4b5563"));
        d.setFont(Font.font("Arial", 11));

        VBox box = new VBox(4, c, t, r, d);
        box.setAlignment(Pos.CENTER);
        box.setPrefWidth(140);
        box.setStyle("-fx-cursor: hand;");
        return box;
    }

    private Label flowTag(String text, double x, double y, String color) {
        Label tag = new Label(text);
        tag.setLayoutX(x);
        tag.setLayoutY(y);
        tag.setStyle("-fx-background-color: " + color + "; -fx-text-fill: white; -fx-font-weight: bold; -fx-padding: 6 10; -fx-background-radius: 14;");
        return tag;
    }

    private Line line(double sx, double sy, double ex, double ey) {
        Line line = new Line(sx, sy, ex, ey);
        line.setStroke(Color.web("#94a3b8"));
        line.setStrokeWidth(4);
        return line;
    }

    private Label chip(String text, String bg, String fg) {
        Label label = new Label(text);
        label.setStyle(chipStyle(bg, fg));
        label.setFont(Font.font("Arial", FontWeight.BOLD, 13));
        return label;
    }

    private String chipStyle(String bg, String fg) {
        return "-fx-background-color: " + bg + "; -fx-text-fill: " + fg + "; -fx-padding: 8 12; -fx-background-radius: 18;";
    }

    private Label statCard(String title, String value) {
        Label label = new Label(title + "\n" + value);
        label.setMinWidth(165);
        label.setStyle("-fx-background-color: #f9fafb; -fx-border-color: #d1d5db; -fx-border-width: 2; -fx-background-radius: 12; -fx-border-radius: 12; -fx-padding: 10 14;");
        label.setFont(Font.font("Arial", FontWeight.BOLD, 13));
        return label;
    }

    private String card(String bg) {
        return "-fx-background-color: " + bg + "; -fx-background-radius: 18; -fx-border-radius: 18; ";
    }

    private String border(String color, int width) {
        return "-fx-border-color: " + color + "; -fx-border-width: " + width + ";";
    }

    @FunctionalInterface
    interface JNIAction {
        String run();
    }

    public static void main(String[] args) {
        launch(args);
    }
}
