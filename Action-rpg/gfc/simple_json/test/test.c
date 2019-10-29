#include "simple_json.h"
#include <stdio.h>

int main(int argc, char *argv[])
{

    SJson *json;
    SJson *sub;
    SJson *value;
    int i,count;
    float f;
    printf("Loading Json file %s\n",argv[1]);
    
    if (argv[1])
    {
        json = sj_load(argv[1]);
        
        printf("json loaded:\n");
        if (!json)
        {
            printf("json failed to laod:\n%s\n",sj_get_error());
        }
        sj_echo(json);
        
        printf("\nsaving json to file\n");
        sj_save(json,"output.json");
        
        printf("freeing json structure\n");
        sj_free(json);
    }

    printf("building a new json programatically\n");
    
    json =  sj_object_new();
    
    if (!json)
    {
        printf("failed to make a new json object");
        return 0;
    }

    sub = sj_array_new();
    sj_array_append(sub,sj_new_float(3));
    sj_array_append(sub,sj_new_float(4));
    sj_object_insert(json,"Vector2D",sub);


    printf("json created:\n");
    sj_echo(json);
    
    printf("copying json\n");
    sub = sj_copy(json);
    
    printf("copied json:\n");
    sj_echo(sub);
    
    sj_free(sub);
    
    printf("retrieving data from json:\n");
    sub = sj_object_get_value(json,"Vector2D");
    if (!sub)
    {
        printf("failed to get key 'Vector2D' from json");
        return 0;
    }
    count = sj_array_get_count(sub);
    printf("the Vector2D contained: (");
    for (i = 0; i < count; i++)
    {
        value = sj_array_get_nth(sub,i);
        if (!value)continue;
        if(!sj_get_float_value(value,&f))
        {
            printf("--==expected float==--");
        }

        printf("%f",f);
        if (i + 1 < count)
        {
            printf(",");
        }
    }
    printf(")\n");

    printf("freeing json structure\n");
    sj_free(json);
    
    printf("complete\n");

    return 0;
}

