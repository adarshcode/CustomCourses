// FILE: chapter-12-basic.cs
// Chapter 12 — SOLID in Practice — Basic

using System;
using System.Collections.Generic;
using System.Linq;

// Domain Model - SRP: Task represents task data only
public class Task
{
    public int Id { get; }
    public string Title { get; private set; }
    public string Description { get; private set; }
    public TaskPriority Priority { get; private set; }
    public TaskStatus Status { get; private set; }
    public DateTime CreatedAt { get; }
    public DateTime? CompletedAt { get; private set; }
    
    public Task(int id, string title, string description, TaskPriority priority = TaskPriority.Medium)
    {
        Id = id;
        Title = title;
        Description = description;
        Priority = priority;
        Status = TaskStatus.Pending;
        CreatedAt = DateTime.Now;
    }
    
    public void UpdateTitle(string newTitle)
    {
        Title = newTitle;
    }
    
    public void UpdatePriority(TaskPriority newPriority)
    {
        Priority = newPriority;
    }
    
    public void Complete()
    {
        Status = TaskStatus.Completed;
        CompletedAt = DateTime.Now;
    }
    
    public override string ToString()
    {
        return $"{Id}. {Title} [{Priority} Priority] - {Status}";
    }
}

public enum TaskPriority
{
    Low,
    Medium,
    High
}

public enum TaskStatus
{
    Pending,
    InProgress,
    Completed
}

// Abstractions for DIP
public interface ITaskRepository
{
    void Save(Task task);
    Task GetById(int id);
    List<Task> GetAll();
    void Update(Task task);
    void Delete(int id);
}

public interface INotificationService
{
    void SendNotification(string message);
}

// SRP: Repository handles only data storage
public class MemoryTaskRepository : ITaskRepository
{
    private Dictionary<int, Task> tasks = new Dictionary<int, Task>();
    
    public void Save(Task task)
    {
        tasks[task.Id] = task;
    }
    
    public Task GetById(int id)
    {
        return tasks.ContainsKey(id) ? tasks[id] : null;
    }
    
    public List<Task> GetAll()
    {
        return tasks.Values.ToList();
    }
    
    public void Update(Task task)
    {
        if (tasks.ContainsKey(task.Id))
        {
            tasks[task.Id] = task;
        }
    }
    
    public void Delete(int id)
    {
        tasks.Remove(id);
    }
}

// SRP: Each notification service handles only one type of notification
public class EmailNotificationService : INotificationService
{
    public void SendNotification(string message)
    {
        Console.WriteLine($"Email notification: {message}");
    }
}

public class SMSNotificationService : INotificationService
{
    public void SendNotification(string message)
    {
        Console.WriteLine($"SMS notification: {message}");
    }
}

// SRP: TaskManager handles only task business logic
public class TaskManager
{
    private readonly ITaskRepository repository;
    private readonly INotificationService notificationService;
    private int nextId = 1;
    
    // DIP: Depends on abstractions, not concrete classes
    public TaskManager(ITaskRepository repository, INotificationService notificationService)
    {
        this.repository = repository;
        this.notificationService = notificationService;
    }
    
    public Task CreateTask(string title, string description, TaskPriority priority = TaskPriority.Medium)
    {
        var task = new Task(nextId++, title, description, priority);
        repository.Save(task);
        notificationService.SendNotification($"Task '{task.Title}' created");
        return task;
    }
    
    public void UpdateTaskPriority(int taskId, TaskPriority newPriority)
    {
        var task = repository.GetById(taskId);
        if (task != null)
        {
            task.UpdatePriority(newPriority);
            repository.Update(task);
            notificationService.SendNotification($"Task '{task.Title}' priority updated to {newPriority}");
        }
    }
    
    public void CompleteTask(int taskId)
    {
        var task = repository.GetById(taskId);
        if (task != null)
        {
            task.Complete();
            repository.Update(task);
            notificationService.SendNotification($"Task '{task.Title}' status changed to {task.Status}");
        }
    }
    
    public List<Task> GetAllTasks()
    {
        return repository.GetAll();
    }
}

class Program
{
    static void Main()
    {
        Console.WriteLine("=== SOLID Task Manager Demo ===");
        
        // DIP: Inject dependencies (can easily switch implementations)
        ITaskRepository repository = new MemoryTaskRepository();
        INotificationService emailService = new EmailNotificationService();
        
        var taskManager = new TaskManager(repository, emailService);
        
        // Create some tasks
        var task1 = taskManager.CreateTask("Complete project documentation", "Write comprehensive documentation");
        var task2 = taskManager.CreateTask("Buy groceries", "Milk, bread, eggs");
        
        // Update and complete tasks
        taskManager.UpdateTaskPriority(task1.Id, TaskPriority.High);
        taskManager.CompleteTask(task2.Id);
        
        // Display all tasks
        Console.WriteLine("\n=== All Tasks ===");
        var allTasks = taskManager.GetAllTasks();
        foreach (var task in allTasks)
        {
            Console.WriteLine(task);
        }
        
        Console.WriteLine("\n=== Demonstrating OCP: Easy to Add New Notification Service ===");
        
        // OCP: Can easily add new notification service without modifying existing code
        INotificationService smsService = new SMSNotificationService();
        var taskManagerWithSMS = new TaskManager(repository, smsService);
        
        var task3 = taskManagerWithSMS.CreateTask("Call dentist", "Schedule appointment");
        taskManagerWithSMS.CompleteTask(task3.Id);
        
        Console.WriteLine("\n=== SOLID Principles Demonstrated ===");
        Console.WriteLine("✓ SRP: Each class has one responsibility");
        Console.WriteLine("✓ OCP: Easy to add new notification types");
        Console.WriteLine("✓ LSP: All repositories work interchangeably");
        Console.WriteLine("✓ ISP: Interfaces are focused and specific");
        Console.WriteLine("✓ DIP: High-level modules depend on abstractions");
    }
}