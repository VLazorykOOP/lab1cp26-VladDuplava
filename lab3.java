import java.util.ArrayList;
import java.util.List;

// --- 1. PATTERN: PROTOTYPE ---
// Базовий клас датчика з можливістю клонування
abstract class WeatherSensor implements Cloneable {
    protected String model;
    protected double temperature;

    public void setTemperature(double temperature) { this.temperature = temperature; }
    public abstract void showInfo();

    @Override
    public WeatherSensor clone() {
        try {
            return (WeatherSensor) super.clone();
        } catch (CloneNotSupportedException e) {
            return null;
        }
    }
}

class DefaultSensor extends WeatherSensor {
    public DefaultSensor(String model) { this.model = model; }
    @Override
    public void showInfo() {
        System.out.println("Датчик [" + model + "] показує: " + temperature + "°C");
    }
}

// --- 2. PATTERN: ADAPTER ---
// Старий сторонній прилад, який ми не можемо змінювати
class LegacyThermometer {
    public double getTempFahrenheit() { return 80.6; } // Повертає Фаренгейти
}

// Адаптер, який робить старий прилад сумісним з нашою системою (WeatherSensor)
class SensorAdapter extends WeatherSensor {
    private LegacyThermometer legacyDevice;

    public SensorAdapter(LegacyThermometer device) {
        this.legacyDevice = device;
        this.model = "Адаптований-Старий-Датчик";
    }

    @Override
    public void showInfo() {
        // Конвертація: (F - 32) * 5/9 = C
        double celsius = (legacyDevice.getTempFahrenheit() - 32) * 5 / 9;
        System.out.println("Датчик [" + model + "] (через адаптер): " + String.format("%.1f", celsius) + "°C");
    }
}

// --- 3. PATTERN: OBSERVER ---
interface Display {
    void update(double temp);
}

class WeatherStation {
    private List<Display> displays = new ArrayList<>();

    public void addDisplay(Display d) { displays.add(d); }

    public void changeTemperature(double newTemp) {
        System.out.println("\n--- Станція: Температура змінилася до " + newTemp + "°C ---");
        for (Display d : displays) {
            d.update(newTemp);
        }
    }
}

class PhoneApp implements Display {
    @Override
    public void update(double temp) {
        System.out.println("[Додаток] Сповіщення: Поточна температура зараз " + temp + "°C");
    }
}

// --- ГОЛОВНИЙ КЛАС ---
public class DesignPatternsDemo {
    public static void main(String[] args) {

        // Демо Prototype: клонуємо налаштований датчик
        DefaultSensor prototypeSensor = new DefaultSensor("SmartSensor-v1");
        DefaultSensor kitchenSensor = (DefaultSensor) prototypeSensor.clone();
        kitchenSensor.setTemperature(22.0);
        kitchenSensor.showInfo();

        // Демо Adapter: використовуємо старий прилад як новий датчик
        LegacyThermometer oldTerm = new LegacyThermometer();
        WeatherSensor adapted = new SensorAdapter(oldTerm);
        adapted.showInfo();

        // Демо Observer: підписуємо додаток на оновлення станції
        WeatherStation station = new WeatherStation();
        PhoneApp myiPhone = new PhoneApp();

        station.addDisplay(myiPhone);
        station.changeTemperature(25.5); // Додаток автоматично отримає оновлення
    }
}
