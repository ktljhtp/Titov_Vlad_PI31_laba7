import java.util.ArrayList;
import java.util.Scanner;

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

        // Демонстрация производного класса, вызова конструктора базового класса
        AudioContent audio = new AudioContent("Classical Music", 300, "MP3");
        VideoContent video = new VideoContent("Nature Documentary", 1200, "1080p");

        // Демонстрация абстрактного класса
        MediaContent[] mediaLibrary = {audio, video};
        for (MediaContent media : mediaLibrary) {
            media.printInfo();
            media.play();
            System.out.println();
        }

        // Демонстрация интерфейса
        Playable track = new MusicTrack("Shape of You", "Ed Sheeran");
        track.play();
        track.pause();
        track.stop();

        // Создание пользователя
        User user = new User("Alice");
        user.addMedia(audio);
        user.addMedia(video);
        user.printUserInfo();

        // Демонстрация клонирования
        Playlist originalPlaylist = new Playlist();
        originalPlaylist.setName("My Playlist");
        originalPlaylist.addContent(audio);
        originalPlaylist.addContent(video);

        try {
            Playlist clonedPlaylist = (Playlist) originalPlaylist.clone();
            Playlist deepClonedPlaylist = originalPlaylist.deepClone();

            System.out.println("\nOriginal Playlist:");
            originalPlaylist.printPlaylistInfo();

            System.out.println("\nShallow Cloned Playlist:");
            clonedPlaylist.printPlaylistInfo();

            System.out.println("\nDeep Cloned Playlist:");
            deepClonedPlaylist.printPlaylistInfo();
        } catch (CloneNotSupportedException e) {
            System.out.println("Cloning failed: " + e.getMessage());
        }

        scanner.close();
    }
}
