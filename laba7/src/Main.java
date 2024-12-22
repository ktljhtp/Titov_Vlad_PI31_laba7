import java.util.ArrayList;
import java.util.Scanner;
import java.util.Collections;
import java.util.Comparator;

// Абстрактный класс для медиа-контента
abstract class MediaContent {
    protected String title;
    protected float duration;

    public MediaContent(String title, float duration) {
        this.title = title;
        this.duration = duration;
    }

    public abstract void play();

    public void printInfo() {
        System.out.println("Title: " + title + ", Duration: " + duration + " sec");
    }
}

// Производный класс для аудио-контента
class AudioContent extends MediaContent {
    private String audioFormat;

    public AudioContent(String title, float duration, String audioFormat) {
        super(title, duration); // Вызов конструктора базового класса
        this.audioFormat = audioFormat;
    }

    @Override
    public void play() {
        System.out.println("Playing audio: " + title + " in format " + audioFormat);
    }

    @Override
    public void printInfo() {
        System.out.println("Audio Title: " + title + ", Duration: " + duration + " sec, Format: " + audioFormat);
    }
}

// Производный класс для видео-контента
class VideoContent extends MediaContent {
    private String resolution;

    public VideoContent(String title, float duration, String resolution) {
        super(title, duration); // Вызов конструктора базового класса
        this.resolution = resolution;
    }

    @Override
    public void play() {
        System.out.println("Playing video: " + title + " in resolution " + resolution);
    }

    @Override
    public void printInfo() {
        super.printInfo(); // Вызов метода базового класса
        System.out.println("Resolution: " + resolution);
    }
}

// Интерфейс Playable
interface Playable {
    void play();

    void pause();

    void stop();
}

// Класс для музыкального трека, реализующий интерфейс
class MusicTrack implements Playable {
    private String title;
    private String artist;

    public MusicTrack(String title, String artist) {
        this.title = title;
        this.artist = artist;
    }

    @Override
    public void play() {
        System.out.println("Playing music: " + title + " by " + artist);
    }

    @Override
    public void pause() {
        System.out.println("Pausing music: " + title);
    }

    @Override
    public void stop() {
        System.out.println("Stopping music: " + title);
    }
}

// Класс для плейлиста
class Playlist implements Cloneable {
    private String name;
    private ArrayList<MediaContent> mediaContents = new ArrayList<>();

    public void setName(String name) {
        this.name = name;
    }

    public void addContent(MediaContent content) {
        mediaContents.add(content);
    }

    public void printPlaylistInfo() {
        System.out.println("Playlist: " + name);
        for (MediaContent content : mediaContents) {
            content.printInfo();
        }
    }

    @Override
    protected Object clone() throws CloneNotSupportedException {
        return super.clone(); // Мелкое клонирование
    }

    public Playlist deepClone() throws CloneNotSupportedException {
        Playlist clonedPlaylist = (Playlist) super.clone();
        clonedPlaylist.mediaContents = new ArrayList<>(mediaContents); // Глубокое клонирование
        return clonedPlaylist;
    }
}

// Класс пользователя
class User {
    private String username;
    private Playlist playlist;

    public User(String username) {
        this.username = username;
        this.playlist = new Playlist();
        playlist.setName(username + "'s Playlist");
    }

    public void addMedia(MediaContent content) {
        playlist.addContent(content);
    }

    public void printUserInfo() {
        System.out.println("User: " + username);
        playlist.printPlaylistInfo();
    }
}

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        // Контейнер для хранения медиа-контента
        ArrayList<MediaContent> mediaLibrary = new ArrayList<>();

        // Добавление объектов базового и производного классов
        mediaLibrary.add(new AudioContent("Classical Music", 300, "MP3"));
        mediaLibrary.add(new VideoContent("Nature Documentary", 1200, "1080p"));
        mediaLibrary.add(new AudioContent("Jazz", 180, "WAV"));
        mediaLibrary.add(new VideoContent("Sci-Fi Movie", 7200, "4K"));

        // Печать исходного списка
        System.out.println("Original Media Library:");
        printMediaLibrary(mediaLibrary);

        // Сортировка медиа-контента по продолжительности
        mediaLibrary.sort(Comparator.comparingDouble(media -> media.duration));
        System.out.println("\nMedia Library Sorted by Duration:");
        printMediaLibrary(mediaLibrary);

        // Поиск контента по названию
        System.out.print("\nEnter the title to search: ");
        String searchTitle = scanner.nextLine();
        MediaContent foundContent = searchByTitle(mediaLibrary, searchTitle);

        if (foundContent != null) {
            System.out.println("\nFound Content:");
            foundContent.printInfo();
        } else {
            System.out.println("\nNo content found with title: " + searchTitle);
        }

        scanner.close();
    }

    // Метод для печати медиатеки
    private static void printMediaLibrary(ArrayList<MediaContent> library) {
        for (MediaContent media : library) {
            media.printInfo();
        }
    }

    // Метод для поиска контента по названию
    private static MediaContent searchByTitle(ArrayList<MediaContent> library, String title) {
        for (MediaContent media : library) {
            if (media.title.equalsIgnoreCase(title)) {
                return media;
            }
        }
        return null;
    }
}